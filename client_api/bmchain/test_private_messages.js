var pkg = require('./package.json');
var aes = require('./steem/lib/auth/ecc/src/aes');
var memo = require('./steem/lib/auth/memo.js')
const bmchain = require("./steem/lib");

var public_key = require("./steem/lib/auth/ecc/src/key_public.js");
var private_key = require("./steem/lib/auth/ecc/src/key_private.js");

var send_private_message = true;
var get_inbox = true;
var get_outbox = true;

// #1: send private message
if (send_private_message) {
    (async () => {
        try {
            const priv_memo_user001 = '5J2wPb6BVAyZZYpmJHRks1DgCLEHRmzAyrwzbsvX3xaHibH8XLM';
            const publ_memo_user002 = 'BMT58GceeEnG9TRDHjGcnTY3sWfPtwKEdD6kemCyuTihfsma7Db9d';

            const priv_memo_user002 = '5KDkjj6MV2EsoabJqMqfVrXz2hnsmsPFyqiycyduy5ycKjNRYpD';
            const publ_memo_user001 = 'BMT7o4Az56RoUhLay68NaUFh56sgCjStvEBt7kGzVGBqPuuQjgbpr';

            const timestamp = (new Date).getTime();
            const encrypt_message = aes.encrypt(
                priv_memo_user001,
                publ_memo_user002,
                'I go it!');

            const decrypt_message = aes.decrypt(
                priv_memo_user002,
                publ_memo_user001,
                encrypt_message.nonce,
                encrypt_message.message,
                encrypt_message.checksum
            );

            console.log(decrypt_message.toString('utf8'))

            const operations = [
                ['private_message',
                    {
                        from: 'user001',
                        to: 'user002',
                        from_memo_key: public_key.fromString('BMT7o4Az56RoUhLay68NaUFh56sgCjStvEBt7kGzVGBqPuuQjgbpr'),
                        to_memo_key: public_key.fromString('BMT58GceeEnG9TRDHjGcnTY3sWfPtwKEdD6kemCyuTihfsma7Db9d'),
                        sent_time: encrypt_message.nonce,
                        checksum: encrypt_message.checksum,
                        message_size: encrypt_message.message.toString('hex').length,
                        encrypted_message: encrypt_message.message.toString('hex')
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