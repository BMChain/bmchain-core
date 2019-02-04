var pkg = require('./package.json');
var aes = require('./steem/lib/auth/ecc/src/aes');
const bmchain = require("./steem/lib");
var _types = require("./steem/lib/auth/serializer/src/types");
var _types2 = _interopRequireDefault(_types);
var ByteBuffer = require('bytebuffer');
var Long = require('long');
uint64 = _types2.default.uint64;

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var encrypted_content = true;
var approve_content_order = false;
var get_brought_encrypted_content = false;

// Name account: user001
// Private owner key  : 5J5bXq2eXvddekkLNSp85XBkrK3CGRyo1fSCP5Q3PgEoh5BsrNB
// Public owner key   : BMT4zjx55qKc6BphoVqNLDrtwML7RSXLGokfzWpnwSgcha7jBH7h7
// Private active key : 5HzQgs6Z33Ru8T4kqFZbPw9qwuhQAap7gA6VP22LK7btZz38btV
// Public active key  : BMT7aLD9yK75LaHo9fubJb8xkALTPj5y5L4U6YCnuDSnegn4N8Jm1
// Private posting key: 5Jn4xHEksEPtDheWzoUGq5wyicCgPb1iuvce5enQzmhWQieytMs
// Public posting key : BMT8YzkXTtFi8cpsWpPZBxvLmZfefWdZ6hzpK1tZbNJoc1ndZPFwd
// Private memo key   : 5K1exEzxbg1gkZBqBP2S7zM31Mhku9TXJubsdvFeAMHNoG4RY2c
// Public memo key    : BMT5mvxq7mQrjGtyVQQ7uZh4XNzN5LjoJ49kzxc6JE5twfBL3mgs9

// Name account: user002
// Private owner key  : 5K5RHExhyXxP3EFinzurQhxVtggPbFm1fwHtWtVp3HjdPwQ5fWa
// Public owner key   : BMT5e7GB21HpNCcP3j7HYptCRZpTVVWAVaE1XoktgycmpvauSUHxP
// Private active key : 5Hxez51taPrNmQCbzFuDPuzF1kw68zjxx1yCiZB2k2rjpJ2moa5
// Public active key  : BMT56QrZqA4Wbxg4umFS5eEU5KZtSwkyDqpirmXLcAZZMrxKJS1pL
// Private posting key: 5JTM96uunx9MS5oYeSKfqAA9s9hQ4gZK7ryDLG75u6SVky3qwsR
// Public posting key : BMT6Ztegfi35FRUFmgxisUN9DqoyURpMqKwkiBjT1DJXH6wZJL4Ao
// Private memo key   : 5Js7QFLZ1kUxkWA42Hjf9vipuKvjniXYCVjNJWQi52Sndohf5wX
// Public memo key    : BMT774HWEajgLMjNmf1QdD5NFH96yaEf69XvWEE2qvVYAt2zcZyfE

priv_memo_user001 = '5K1exEzxbg1gkZBqBP2S7zM31Mhku9TXJubsdvFeAMHNoG4RY2c';
publ_memo_user001 = 'BMT5mvxq7mQrjGtyVQQ7uZh4XNzN5LjoJ49kzxc6JE5twfBL3mgs9';
priv_memo_user002 = '5Js7QFLZ1kUxkWA42Hjf9vipuKvjniXYCVjNJWQi52Sndohf5wX';
publ_memo_user002 = 'BMT774HWEajgLMjNmf1QdD5NFH96yaEf69XvWEE2qvVYAt2zcZyfE';

if (encrypted_content) {
    (async () => {
        var permlink = bmchain.formatter.commentPermlink('', 'encrypted-post-001-001');
        try {
            // #1: posting encrypted content
            const encrypt_message = aes.encrypt(
                priv_memo_user001,
                publ_memo_user001,
                'Don\'t cry because it\'s over, smile because it happened.');

            const decrypt_message = aes.decrypt(
                priv_memo_user001,
                publ_memo_user001,
                encrypt_message.nonce,
                encrypt_message.message,
                encrypt_message.checksum
            );

            console.log(decrypt_message.toString('utf8'))

            const operations = [
                ['encrypted_content',
                    {
                        parent_author: '',
                        parent_permlink: 'encrypted',
                        author: 'user001',
                        permlink: permlink,
                        title: 'title',
                        body: 'body',
                        json_metadata: JSON.stringify({
                            tags: ['test'],
                            app: `steemjs/${pkg.version}`,
                            nonce: encrypt_message.nonce.toString(),
                            message_size: encrypt_message.message.length
                        }),
                        encrypted_message: encrypt_message.message.toString('hex'),
                        sent_time: encrypt_message.nonce,
                        nonce: encrypt_message.nonce,
                        message_size: encrypt_message.message.length,
                        checksum: encrypt_message.checksum,
                        price: bmchain.formatter.amount(1.000, 'BMT'),
                        owner: '',
                        order_id: 0,
                        apply_order: false
                    }
                ]
            ];

            const tx = await bmchain.broadcast.sendAsync(
                {operations, extensions: []},
                {posting: '5Jn4xHEksEPtDheWzoUGq5wyicCgPb1iuvce5enQzmhWQieytMs'}
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

        // #2: get list of encrypted contents
        try {
            const ret = await bmchain.api.getEncryptedDiscussions({
                tag: 'user001',
                owner: '',
                limit: 5,
                truncate_body: 1024
            });

            permlink = ret["0"].permlink;

            // const nonce = Long.fromString(JSON.parse(ret["0"].json_metadata)['nonce']);
            // const mes_size = parseInt(JSON.parse(ret["0"].json_metadata)['message_size']);
            const nonce = ret["0"].nonce;
            const mes_size = ret["0"].message_size;
            const checksum = parseInt(ret["0"].checksum);
            const enc_mes = new Buffer(mes_size);
            enc_mes.write(ret["0"].encrypted_body, 'hex');
            const decrypt_message = aes.decrypt(
                priv_memo_user001,
                publ_memo_user001,
                nonce,
                enc_mes,
                checksum);
            ret[0].decrypted_content = decrypt_message.toString('utf8');
            console.log(ret[0]);
        } catch (e) {
            console.error(e);
        }

        // #3: order to buy
        try {
            const operations = [
                ['content_order_create',
                    {
                        author: 'user001',
                        permlink: permlink,
                        owner: 'user002',
                        price: '1.000 BMT',
                        json_metadata: JSON.stringify({
                            tags: ['test'],
                            app: `steemjs/${pkg.version}`
                        })
                    }
                ]
            ];

            const tx = await bmchain.broadcast.sendAsync(
                {operations, extensions: []},
                {active: '5Hxez51taPrNmQCbzFuDPuzF1kw68zjxx1yCiZB2k2rjpJ2moa5'}
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

        // #4: get content orders
        try {
            const ret = await bmchain.api.getContentOrders('user002', 'user001', -1, 10);
            console.log(ret);
        } catch (e) {
            console.error(e);
        }
    })();
}

// #5: approve order
if (approve_content_order) {
    (async () => {
        try {
            const orders = await bmchain.api.getContentOrders('user002', 'user001', -1, 10);
            const posts = await bmchain.api.getEncryptedDiscussions({tag: 'user001', owner: '', limit: 1, truncate_body: 1024});
            const order = orders[0]
            const post = posts[0]

            const nonce = Long.fromString(JSON.parse(post.json_metadata)['nonce']);
            const checksum = parseInt(post.checksum);
            const mes_size = parseInt(JSON.parse(post.json_metadata)['message_size']);
            const enc_mes = new Buffer(mes_size);
            enc_mes.write(post.encrypted_body, 'hex');
            const decrypt_message = aes.decrypt(
                priv_memo_user001,
                publ_memo_user001,
                nonce,
                enc_mes,
                checksum);
            decrypted_content = decrypt_message.toString('utf8');

            console.log(decrypted_content)

            const encrypt_post = aes.encrypt(
                priv_memo_user001,
                publ_memo_user002,
                decrypted_content);

            const decrypt_post = aes.decrypt(
                priv_memo_user002,
                publ_memo_user001,
                encrypt_post.nonce,
                encrypt_post.message,
                encrypt_post.checksum
            );

            console.log(decrypt_post.toString('utf8'))

            const permlink2 = bmchain.formatter.commentPermlink('', 'encrypted-post-001-001');

            const operations = [
                ['encrypted_content',
                    {
                        parent_author: '',
                        parent_permlink: 'encrypted',
                        author: 'user001',
                        permlink: permlink2,
                        title: post['title'],
                        body: post['body'],
                        json_metadata: JSON.stringify({
                            tags: ['test'],
                            app: `steemjs/${pkg.version}`,
                            nonce: encrypt_post.nonce.toString(),
                            message_size: encrypt_post.message.length
                        }),
                        encrypted_message: encrypt_post.message.toString('hex'),
                        sent_time: encrypt_post.nonce,
                        message_size: encrypt_post.message.length,
                        checksum: encrypt_post.checksum,
                        price: bmchain.formatter.amount(0.000, 'BMT'),
                        owner: order['owner'],
                        order_id: order['id'],
                        apply_order: true
                    }
                ]
            ];

            const tx = await bmchain.broadcast.sendAsync(
                {operations, extensions: []},
                {posting: '5JTnDZznCiwYNefL8RsdEvhbqqAqyZpvWWkJh2hHiX2CXXUZg4d'}
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

// #6: get brought content and decrypt it
if (get_brought_encrypted_content){
    (async () => {
        try {
            const posts = await bmchain.api.getEncryptedDiscussions({
                tag: 'user001',
                owner: 'user002',
                limit: 1,
                truncate_body: 1024
            });
            const post = posts[0];
            const nonce = Long.fromString(JSON.parse(post.json_metadata)['nonce']);
            const checksum = parseInt(post.checksum);
            const mes_size = parseInt(JSON.parse(post.json_metadata)['message_size']);
            const enc_mes = new Buffer(mes_size);
            enc_mes.write(post.encrypted_body, 'hex');
            const decrypt_message = aes.decrypt(
                priv_memo_user002,
                publ_memo_user001,
                nonce,
                enc_mes,
                checksum);

            post.decrypted_content = decrypt_message.toString('utf8');

            console.log(post)

            console.log(ret);
        } catch (e) {
            console.error(e);
        }
    })();
}