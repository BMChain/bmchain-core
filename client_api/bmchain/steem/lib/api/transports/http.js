'use strict';

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _fetchPonyfill2 = require('fetch-ponyfill');

var _fetchPonyfill3 = _interopRequireDefault(_fetchPonyfill2);

var _bluebird = require('bluebird');

var _bluebird2 = _interopRequireDefault(_bluebird);

var _debug = require('debug');

var _debug2 = _interopRequireDefault(_debug);

var _base = require('./base');

var _base2 = _interopRequireDefault(_base);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

var _fetchPonyfill = (0, _fetchPonyfill3.default)(_bluebird2.default),
    fetch = _fetchPonyfill.fetch;

var debug = (0, _debug2.default)('steem:http');

var HttpTransport = function (_Transport) {
  _inherits(HttpTransport, _Transport);

  function HttpTransport() {
    _classCallCheck(this, HttpTransport);

    return _possibleConstructorReturn(this, (HttpTransport.__proto__ || Object.getPrototypeOf(HttpTransport)).apply(this, arguments));
  }

  _createClass(HttpTransport, [{
    key: 'send',
    value: function send(api, data, callback) {
      debug('Steem::send', api, data);
      var id = data.id || this.id++;
      var payload = {
        id: id,
        jsonrpc: '2.0',
        method: 'call',
        params: [api, data.method, data.params]
      };
      fetch(this.options.uri, {
        method: 'POST',
        body: JSON.stringify(payload)
      }).then(function (res) {
        debug('Steem::receive', api, data);
        return res.json();
      }).then(function (json) {
        var err = json.error || '';
        var result = json.result || '';
        callback(err, result);
      }).catch(function (err) {
        callback(err, '');
      });
    }
  }]);

  return HttpTransport;
}(_base2.default);

exports.default = HttpTransport;