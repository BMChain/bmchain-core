'use strict';

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

var _bluebird = require('bluebird');

var _bluebird2 = _interopRequireDefault(_bluebird);

var _debug = require('debug');

var _debug2 = _interopRequireDefault(_debug);

var _helpers = require('./helpers');

var _helpers2 = _interopRequireDefault(_helpers);

var _formatter = require('../formatter');

var _formatter2 = _interopRequireDefault(_formatter);

var _operations = require('./operations');

var _operations2 = _interopRequireDefault(_operations);

var _api = require('../api');

var _api2 = _interopRequireDefault(_api);

var _auth = require('../auth');

var _auth2 = _interopRequireDefault(_auth);

var _utils = require('../utils');

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var debug = (0, _debug2.default)('steem:broadcast');
var noop = function noop() {};
var formatter = (0, _formatter2.default)(_api2.default);

var steemBroadcast = {};

// Base transaction logic -----------------------------------------------------

/**
 * Sign and broadcast transactions on the steem network
 */

steemBroadcast.send = function steemBroadcast$send(tx, privKeys, callback) {
  var resultP = steemBroadcast._prepareTransaction(tx).then(function (transaction) {
    debug('Signing transaction (transaction, transaction.operations)', transaction, transaction.operations);
    return _bluebird2.default.join(transaction, _auth2.default.signTransaction(transaction, privKeys));
  }).spread(function (transaction, signedTransaction) {
    debug('Broadcasting transaction (transaction, transaction.operations)', transaction, transaction.operations);
    return _api2.default.broadcastTransactionSynchronousAsync(signedTransaction).then(function (result) {
      return Object.assign({}, result, signedTransaction);
    });
  });

  resultP.nodeify(callback || noop);
};

steemBroadcast._prepareTransaction = function steemBroadcast$_prepareTransaction(tx) {
  var propertiesP = _api2.default.getDynamicGlobalPropertiesAsync();
  return propertiesP.then(function (properties) {
    // Set defaults on the transaction
    var chainDate = new Date(properties.time + 'Z');
    var refBlockNum = properties.head_block_number - 3 & 0xFFFF;
    return _api2.default.getBlockAsync(properties.head_block_number - 2).then(function (block) {
      var headBlockId = block.previous;
      return Object.assign({
        ref_block_num: refBlockNum,
        ref_block_prefix: new Buffer(headBlockId, 'hex').readUInt32LE(4),
        expiration: new Date(chainDate.getTime() + 60 * 1000)
      }, tx);
    });
  });
};

// Generated wrapper ----------------------------------------------------------

// Generate operations from operations.json
_operations2.default.forEach(function (operation) {
  var operationName = (0, _utils.camelCase)(operation.operation);
  var operationParams = operation.params || [];

  var useCommentPermlink = operationParams.indexOf('parent_permlink') !== -1 && operationParams.indexOf('parent_permlink') !== -1;

  steemBroadcast[operationName + 'With'] = function steemBroadcast$specializedSendWith(wif, options, callback) {
    debug('Sending operation "' + operationName + '" with', { options: options, callback: callback });
    var keys = {};
    if (operation.roles && operation.roles.length) {
      keys[operation.roles[0]] = wif; // TODO - Automatically pick a role? Send all?
    }
    return steemBroadcast.send({
      extensions: [],
      operations: [[operation.operation, Object.assign({}, options, options.json_metadata != null ? {
        json_metadata: toString(options.json_metadata)
      } : {}, useCommentPermlink && options.permlink == null ? {
        permlink: formatter.commentPermlink(options.parent_author, options.parent_permlink)
      } : {})]]
    }, keys, callback);
  };

  steemBroadcast[operationName] = function steemBroadcast$specializedSend(wif) {
    for (var _len = arguments.length, args = Array(_len > 1 ? _len - 1 : 0), _key = 1; _key < _len; _key++) {
      args[_key - 1] = arguments[_key];
    }

    debug('Parsing operation "' + operationName + '" with', { args: args });
    var options = operationParams.reduce(function (memo, param, i) {
      memo[param] = args[i]; // eslint-disable-line no-param-reassign
      return memo;
    }, {});
    var callback = args[operationParams.length];
    return steemBroadcast[operationName + 'With'](wif, options, callback);
  };
});

var toString = function toString(obj) {
  return (typeof obj === 'undefined' ? 'undefined' : _typeof(obj)) === 'object' ? JSON.stringify(obj) : obj;
};
(0, _helpers2.default)(steemBroadcast);

_bluebird2.default.promisifyAll(steemBroadcast);

exports = module.exports = steemBroadcast;