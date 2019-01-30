var pkg = require('./package.json');
var aes = require('./steem/lib/auth/ecc/src/aes');
const bmchain = require("./steem/lib");
var _types = require("./steem/lib/auth/serializer/src/types");
var _types2 = _interopRequireDefault(_types);
var ByteBuffer = require('bytebuffer');
var Long = require('long');
uint64 = _types2.default.uint64;

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var encrypted_content = false;
var approve_content_order = false;
var get_brought_encrypted_content = true;

// Name account: user001
// Private owner key  : 5JZnth3gttgX67j8jdP5BvXZXbEZZU9PKnoUgS8XFmqfboc59LD
// Public owner key   : BMT6Jyi5X9p1L5ou9iVB6qq7nUHpikXgcEiW1X7hB1oofsv38g6Yd
// Private active key : 5KaXtAD4YWCzp1KksyC2rVTDbgr6BcyvRw4n6zQ9W6rrSjavQUT
// Public active key  : BMT6iGC8BNBx8Dd389qrFVEvGXLwbNCyPNBCiZ9dyotwbCRVNXN3K
// Private posting key: 5JTnDZznCiwYNefL8RsdEvhbqqAqyZpvWWkJh2hHiX2CXXUZg4d
// Public posting key : BMT6AEeYUz7jCbKBxVyoFPfGvtjfyiXDp5W638C5swzQ5QnZUqoNo
// Private memo key   : 5K8YTCRuCV3Jm1pJjH8BTf6GcHKSgF6gAxzk5p8R7b8th8vXoMs
// Public memo key    : BMT8YQ2219mg51pThHYMBDhTMm9JuaUH2Tg4GSuQ9da2U4ddJdd2n

// Name account: user002
// Private owner key  : 5KR47BtUq1QXEabTyj5Rm17bVWGzN3pM5dqBQpiVhKfxnUdC1Pk
// Public owner key   : BMT7tvCtDCx2HoPxcHimtVtoDXT1YbwgjaWjEqomL1GQn23gdYLXA
// Private active key : 5KbjQGhsj2bTsQ7De8EMj1AzBJJi9q5BJuZiszDRL8ge43rxwTU
// Public active key  : BMT6mezynnxdCBHEbd7DairHFC4wHaC7NZpb66w17qMZ4Ko24FgKj
// Private posting key: 5KEv9Qfd82GpJC74kUN76mB2pxw8QQ6HRBgcP69Mdsopnxuzf1T
// Public posting key : BMT5qBkzyGLxyH1qh9JC8MpHHrw47NCwLHFQ82U7nQinEx8qFZKdP
// Private memo key   : 5KaVTgYtUuPFLndgGvuQ3LFCvxZmUaKYxuLK9kR9XggDgp2ch6d
// Public memo key    : BMT6etFjYGXc8fsLpSFNB9yEnk9KKxpDTKdBXGqwmU2tZKUh3CRpg

priv_memo_user001 = '5K8YTCRuCV3Jm1pJjH8BTf6GcHKSgF6gAxzk5p8R7b8th8vXoMs';
publ_memo_user001 = 'BMT8YQ2219mg51pThHYMBDhTMm9JuaUH2Tg4GSuQ9da2U4ddJdd2n';
priv_memo_user002 = '5KaVTgYtUuPFLndgGvuQ3LFCvxZmUaKYxuLK9kR9XggDgp2ch6d';
publ_memo_user002 = 'BMT6etFjYGXc8fsLpSFNB9yEnk9KKxpDTKdBXGqwmU2tZKUh3CRpg';

if (encrypted_content) {
    (async () => {
        const permlink = bmchain.formatter.commentPermlink('', 'encrypted-post-001-001');
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

        // #2: get list of encrypted contents
        try {
            const ret = await bmchain.api.getEncryptedDiscussions({
                tag: 'user001',
                owner: '',
                limit: 5,
                truncate_body: 1024
            });

            const nonce = Long.fromString(JSON.parse(ret["0"].json_metadata)['nonce']);
            const checksum = parseInt(ret["0"].checksum);
            const mes_size = parseInt(JSON.parse(ret["0"].json_metadata)['message_size']);
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
                {active: '5KbjQGhsj2bTsQ7De8EMj1AzBJJi9q5BJuZiszDRL8ge43rxwTU'}
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