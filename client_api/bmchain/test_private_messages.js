var pkg = require('./package.json');
var aes = require('./steem/lib/auth/ecc/src/aes');
var memo = require('./steem/lib/auth/memo.js')
const bmchain = require("./steem/lib");

var public_key = require("./steem/lib/auth/ecc/src/key_public.js");

var send_private_message = true;
var get_inbox = true;
var get_outbox = true;

// #1: send private message
if (send_private_message) {
    (async () => {
        try {
            const encrypted_message = aes.encrypt(
                '5J2wPb6BVAyZZYpmJHRks1DgCLEHRmzAyrwzbsvX3xaHibH8XLM',
                'BMT58GceeEnG9TRDHjGcnTY3sWfPtwKEdD6kemCyuTihfsma7Db9d',
                'encrypted private message',
                '111111');
            const operations = [
                ['private_message',
                    {
                        from: 'user001',
                        to: 'user002',
                        from_memo_key: public_key.fromString('BMT7o4Az56RoUhLay68NaUFh56sgCjStvEBt7kGzVGBqPuuQjgbpr'),
                        to_memo_key: public_key.fromString('BMT58GceeEnG9TRDHjGcnTY3sWfPtwKEdD6kemCyuTihfsma7Db9d'),
                        sent_time: 111111,
                        checksum: encrypted_message.checksum,
                        message_size: 32,
                        encrypted_message: encrypted_message.message.toString('utf8')
                    }
                ]
            ];

            const tx = await bmchain.broadcast.sendAsync(
                {operations, extensions: []},
                {posting: '5J2srqnG4cKyRdqKPZ9bwmytPmudpQthF96BUA9Sek3cseZjrPR'}
            );

            tx.should.have.properties([
                'expiration',
                'ref_block_num',
                'ref_block_prefix',
                'extensions',
                'operations',
                'signatures',
            ]);
        } catch (e) {
            console.error(e);
        }
    })();
}

// #2: get inbox
if (get_inbox) {
    (async () => {
        try {
            const ret = await bmchain.api.getInbox('user001', "2019-1-1T0:0:0", 10);
            console.log(ret);
        } catch (e) {
            console.error(e);
        }
    })();
    (async () => {
        try {
            const ret = await bmchain.api.getInbox('user002', "2019-1-1T0:0:0", 10);
            console.log(ret);
        } catch (e) {
            console.error(e);
        }
    })();
}

// #3: get outbox
if (get_outbox) {
    (async () => {
        try {
            const ret = await bmchain.api.getOutbox('user001', "2019-1-1T0:0:0", 10);
            console.log(ret);
        } catch (e) {
            console.error(e);
        }
    })();
    (async () => {
        try {
            const ret = await bmchain.api.getOutbox('user002', "2019-1-1T0:0:0", 10);
            console.log(ret);
        } catch (e) {
            console.error(e);
        }
    })();
}