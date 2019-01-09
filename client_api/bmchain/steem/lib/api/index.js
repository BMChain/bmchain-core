'use strict';

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _events = require('events');

var _events2 = _interopRequireDefault(_events);

var _bluebird = require('bluebird');

var _bluebird2 = _interopRequireDefault(_bluebird);

var _config = require('../config');

var _config2 = _interopRequireDefault(_config);

var _methods = require('./methods');

var _methods2 = _interopRequireDefault(_methods);

var _transports = require('./transports');

var _transports2 = _interopRequireDefault(_transports);

var _utils = require('../utils');

var _ecc = require('../auth/ecc');

var _serializer = require('../auth/serializer');

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

var Steem = function (_EventEmitter) {
  _inherits(Steem, _EventEmitter);

  function Steem() {
    var options = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};

    _classCallCheck(this, Steem);

    var _this = _possibleConstructorReturn(this, (Steem.__proto__ || Object.getPrototypeOf(Steem)).call(this, options));

    _this._setTransport(options);
    _this.options = options;
    return _this;
  }

  _createClass(Steem, [{
    key: '_setTransport',
    value: function _setTransport(options) {
      if (options.url.match('^((http|https)?:\/\/)')) {
        options.uri = options.url;
        options.transport = 'http';
        this._transportType = options.transport;
        this.options = options;
        this.transport = new _transports2.default.http(options);
      } else if (options.url.match('^((ws|wss)?:\/\/)')) {
        options.websocket = options.url;
        options.transport = 'ws';
        this._transportType = options.transport;
        this.options = options;
        this.transport = new _transports2.default.ws(options);
      } else if (options.transport) {
        if (this.transport && this._transportType !== options.transport) {
          this.transport.stop();
        }

        this._transportType = options.transport;

        if (typeof options.transport === 'string') {
          if (!_transports2.default[options.transport]) {
            throw new TypeError('Invalid `transport`, valid values are `http`, `ws` or a class');
          }
          this.transport = new _transports2.default[options.transport](options);
        } else {
          this.transport = new options.transport(options);
        }
      } else {
        this.transport = new _transports2.default.ws(options);
      }
    }
  }, {
    key: 'start',
    value: function start() {
      return this.transport.start();
    }
  }, {
    key: 'stop',
    value: function stop() {
      return this.transport.stop();
    }
  }, {
    key: 'send',
    value: function send(api, data, callback) {
      return this.transport.send(api, data, callback);
    }
  }, {
    key: 'setOptions',
    value: function setOptions(options) {
      Object.assign(this.options, options);
      this._setTransport(this.options);
      this.transport.setOptions(this.options);
    }
  }, {
    key: 'setWebSocket',
    value: function setWebSocket(url) {
      this.setOptions({ websocket: url });
    }
  }, {
    key: 'setUri',
    value: function setUri(url) {
      this.setOptions({ uri: url });
    }
  }, {
    key: 'streamBlockNumber',
    value: function streamBlockNumber() {
      var mode = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : 'head';

      var _this2 = this;

      var callback = arguments[1];
      var ts = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : 200;

      if (typeof mode === 'function') {
        callback = mode;
        mode = 'head';
      }
      var current = '';
      var running = true;

      var update = function update() {
        if (!running) return;

        _this2.getDynamicGlobalPropertiesAsync().then(function (result) {
          var blockId = mode === 'irreversible' ? result.last_irreversible_block_num : result.head_block_number;

          if (blockId !== current) {
            if (current) {
              for (var i = current; i < blockId; i++) {
                if (i !== current) {
                  callback(null, i);
                }
                current = i;
              }
            } else {
              current = blockId;
              callback(null, blockId);
            }
          }

          _bluebird2.default.delay(ts).then(function () {
            update();
          });
        }, function (err) {
          callback(err);
        });
      };

      update();

      return function () {
        running = false;
      };
    }
  }, {
    key: 'streamBlock',
    value: function streamBlock() {
      var _this3 = this;

      var mode = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : 'head';
      var callback = arguments[1];

      if (typeof mode === 'function') {
        callback = mode;
        mode = 'head';
      }

      var current = '';
      var last = '';

      var release = this.streamBlockNumber(mode, function (err, id) {
        if (err) {
          release();
          callback(err);
          return;
        }

        current = id;
        if (current !== last) {
          last = current;
          _this3.getBlock(current, callback);
        }
      });

      return release;
    }
  }, {
    key: 'streamTransactions',
    value: function streamTransactions() {
      var mode = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : 'head';
      var callback = arguments[1];

      if (typeof mode === 'function') {
        callback = mode;
        mode = 'head';
      }

      var release = this.streamBlock(mode, function (err, result) {
        if (err) {
          release();
          callback(err);
          return;
        }

        if (result && result.transactions) {
          result.transactions.forEach(function (transaction) {
            callback(null, transaction);
          });
        }
      });

      return release;
    }
  }, {
    key: 'streamOperations',
    value: function streamOperations() {
      var mode = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : 'head';
      var callback = arguments[1];

      if (typeof mode === 'function') {
        callback = mode;
        mode = 'head';
      }

      var release = this.streamTransactions(mode, function (err, transaction) {
        if (err) {
          release();
          callback(err);
          return;
        }

        transaction.operations.forEach(function (operation) {
          callback(null, operation);
        });
      });

      return release;
    }
  }]);

  return Steem;
}(_events2.default);

// Generate Methods from methods.json


_methods2.default.forEach(function (method) {
  var methodName = method.method_name || (0, _utils.camelCase)(method.method);
  var methodParams = method.params || [];

  Steem.prototype[methodName + 'With'] = function Steem$$specializedSendWith(options, callback) {
    var params = methodParams.map(function (param) {
      return options[param];
    });
    return this.send(method.api, {
      method: method.method,
      params: params
    }, callback);
  };

  Steem.prototype[methodName] = function Steem$specializedSend() {
    for (var _len = arguments.length, args = Array(_len), _key = 0; _key < _len; _key++) {
      args[_key] = arguments[_key];
    }

    var options = methodParams.reduce(function (memo, param, i) {
      memo[param] = args[i]; // eslint-disable-line no-param-reassign
      return memo;
    }, {});
    var callback = args[methodParams.length];
    return this[methodName + 'With'](options, callback);
  };
});

/**
 * Wrap transaction broadcast: serializes the object and adds error reporting
 */

Steem.prototype.broadcastTransactionSynchronousWith = function Steem$$specializedSendWith(options, callback) {
  var trx = options.trx;
  return this.send('network_broadcast_api', {
    method: 'broadcast_transaction_synchronous',
    params: [trx]
  }, function (err, result) {
    if (err) {
      var signed_transaction = _serializer.ops.signed_transaction;
      // console.log('-- broadcastTransactionSynchronous -->', JSON.stringify(signed_transaction.toObject(trx), null, 2));
      // toObject converts objects into serializable types

      var trObject = signed_transaction.toObject(trx);
      var buf = signed_transaction.toBuffer(trx);
      err.digest = _ecc.hash.sha256(buf).toString('hex');
      err.transaction_id = buf.toString('hex');
      err.transaction = JSON.stringify(trObject);
      callback(err, '');
    } else {
      callback('', result);
    }
  });
};

_bluebird2.default.promisifyAll(Steem.prototype);

// Export singleton instance
var steem = new Steem(_config2.default);
exports = module.exports = steem;
exports.Steem = Steem;