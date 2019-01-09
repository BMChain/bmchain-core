'use strict';

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _bluebird = require('bluebird');

var _bluebird2 = _interopRequireDefault(_bluebird);

var _defaults = require('lodash/defaults');

var _defaults2 = _interopRequireDefault(_defaults);

var _detectNode = require('detect-node');

var _detectNode2 = _interopRequireDefault(_detectNode);

var _debug = require('debug');

var _debug2 = _interopRequireDefault(_debug);

var _base = require('./base');

var _base2 = _interopRequireDefault(_base);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

var WebSocket = void 0;
if (_detectNode2.default) {
  WebSocket = require('ws'); // eslint-disable-line global-require
} else if (typeof window !== 'undefined') {
  WebSocket = window.WebSocket;
} else {
  throw new Error("Couldn't decide on a `WebSocket` class");
}

var debug = (0, _debug2.default)('steem:ws');

var DEFAULTS = {
  apiIds: {
    database_api: 0,
    login_api: 1,
    follow_api: 3,
    network_broadcast_api: 4,
    private_message_api: 5
  },
  id: 0
};

var WsTransport = function (_Transport) {
  _inherits(WsTransport, _Transport);

  function WsTransport() {
    var options = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};

    _classCallCheck(this, WsTransport);

    (0, _defaults2.default)(options, DEFAULTS);

    var _this = _possibleConstructorReturn(this, (WsTransport.__proto__ || Object.getPrototypeOf(WsTransport)).call(this, options));

    _this.apiIds = options.apiIds;

    _this.inFlight = 0;
    _this.currentP = _bluebird2.default.fulfilled();
    _this.isOpen = false;
    _this.releases = [];
    _this.requests = {};
    _this.requestsTime = {};

    // A Map of api name to a promise to it's API ID refresh call
    _this.apiIdsP = {};
    return _this;
  }

  _createClass(WsTransport, [{
    key: 'start',
    value: function start() {
      var _this2 = this;

      if (this.startP) {
        return this.startP;
      }

      var startP = new _bluebird2.default(function (resolve, reject) {
        if (startP !== _this2.startP) return;
        var url = _this2.options.websocket;
        _this2.ws = new WebSocket(url);

        var releaseOpen = _this2.listenTo(_this2.ws, 'open', function () {
          debug('Opened WS connection with', url);
          _this2.isOpen = true;
          releaseOpen();
          resolve();
        });

        var releaseClose = _this2.listenTo(_this2.ws, 'close', function () {
          debug('Closed WS connection with', url);
          _this2.isOpen = false;
          delete _this2.ws;
          _this2.stop();

          if (startP.isPending()) {
            reject(new Error('The WS connection was closed before this operation was made'));
          }
        });

        var releaseMessage = _this2.listenTo(_this2.ws, 'message', function (message) {
          debug('Received message', message.data);
          var data = JSON.parse(message.data);
          var id = data.id;
          var request = _this2.requests[id];
          if (!request) {
            debug('Steem.onMessage: unknown request ', id);
          }
          delete _this2.requests[id];
          _this2.onMessage(data, request);
        });

        _this2.releases = _this2.releases.concat([releaseOpen, releaseClose, releaseMessage]);
      });

      this.startP = startP;
      this.getApiIds();

      return startP;
    }
  }, {
    key: 'stop',
    value: function stop() {
      debug('Stopping...');
      if (this.ws) this.ws.close();
      this.apiIdsP = {};
      delete this.startP;
      delete this.ws;
      this.releases.forEach(function (release) {
        return release();
      });
      this.releases = [];
    }

    /**
     * Refreshes API IDs, populating the `Steem::apiIdsP` map.
     *
     * @param {String} [requestName] If provided, only this API will be refreshed
     * @param {Boolean} [force] If true the API will be forced to refresh, ignoring existing results
     */

  }, {
    key: 'getApiIds',
    value: function getApiIds(requestName, force) {
      var _this3 = this;

      if (!force && requestName && this.apiIdsP[requestName]) {
        return this.apiIdsP[requestName];
      }

      var apiNamesToRefresh = requestName ? [requestName] : Object.keys(this.apiIds);
      apiNamesToRefresh.forEach(function (name) {
        _this3.apiIdsP[name] = _this3.sendAsync('login_api', {
          method: 'get_api_by_name',
          params: [name]
        }).then(function (result) {
          if (result != null) {
            _this3.apiIds[name] = result;
          }
        });
      });

      // If `requestName` was provided, only wait for this API ID
      if (requestName) {
        return this.apiIdsP[requestName];
      }

      // Otherwise wait for all of them
      return _bluebird2.default.props(this.apiIdsP);
    }
  }, {
    key: 'send',
    value: function send(api, data, callback) {
      var _this4 = this;

      debug('Steem::send', api, data);
      var id = data.id || this.id++;
      var startP = this.start();

      var apiIdsP = api === 'login_api' && data.method === 'get_api_by_name' ? _bluebird2.default.fulfilled() : this.getApiIds(api);

      if (api === 'login_api' && data.method === 'get_api_by_name') {
        debug('Sending setup message');
      } else {
        debug('Going to wait for setup messages to resolve');
      }

      this.currentP = _bluebird2.default.join(startP, apiIdsP).then(function () {
        return new _bluebird2.default(function (resolve, reject) {
          if (!_this4.ws) {
            reject(new Error('The WS connection was closed while this request was pending'));
            return;
          }

          var payload = JSON.stringify({
            id: id,
            method: 'call',
            params: [_this4.apiIds[api], data.method, data.params]
          });

          debug('Sending message', payload);
          _this4.requests[id] = {
            api: api,
            data: data,
            resolve: resolve,
            reject: reject,
            start_time: Date.now()
          };

          // this.inFlight += 1;
          _this4.ws.send(payload);
        });
      }).nodeify(callback);

      return this.currentP;
    }
  }, {
    key: 'onMessage',
    value: function onMessage(message, request) {
      var api = request.api,
          data = request.data,
          resolve = request.resolve,
          reject = request.reject,
          start_time = request.start_time;

      debug('-- Steem.onMessage -->', message.id);
      var errorCause = message.error;
      if (errorCause) {
        var err = new Error(
        // eslint-disable-next-line prefer-template
        (errorCause.message || 'Failed to complete operation') + ' (see err.payload for the full error payload)');
        err.payload = message;
        reject(err);
        return;
      }

      if (api === 'login_api' && data.method === 'login') {
        debug("network_broadcast_api API ID depends on the WS' session. " + 'Triggering a refresh...');
        this.getApiIds('network_broadcast_api', true);
      }

      debug('Resolved', api, data, '->', message);
      this.emit('track-performance', data.method, Date.now() - start_time);
      delete this.requests[message.id];
      resolve(message.result);
    }
  }]);

  return WsTransport;
}(_base2.default);

exports.default = WsTransport;
