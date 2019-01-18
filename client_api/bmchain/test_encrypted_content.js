var pkg = require('./package.json');
var aes = require('./steem/lib/auth/ecc/src/aes');
const bmchain = require("./steem/lib");

var encrypted_content = false;
var get_encrypted_content = false;
var content_order_create = false;
var get_content_orders = false;
var approve_content_order = false;
var get_brought_encrypted_content = false;

// #1: posting encrypted content
if (encrypted_content) {
    (async () => {
        try {
            const permlink = bmchain.formatter.commentPermlink('', 'encrypted');
            const encrypted_message = aes.encrypt(
                '5J2srqnG4cKyRdqKPZ9bwmytPmudpQthF96BUA9Sek3cseZjrPR',
                'BMT7rLYKSpuoukZ5iTraidz432uRFrGLunKP1Zy3v3cLjiGjfJPrN',
                'crypto_body',
                '111111');
            const operations = [
                ['encrypted_content',
                    {
                        parent_author: '',
                        parent_permlink: 'encrypted',
                        author: 'user001',
                        permlink: 'encrypted-post-001-001',
                        title: 'title',
                        body: 'body',
                        json_metadata: JSON.stringify({
                            tags: ['test'],
                            app: `steemjs/${pkg.version}`
                        }),
                        encrypted_message: encrypted_message.message.toString('utf8'),
                        sent_time: 111111,
                        message_size: 32,
                        checksum: encrypted_message.checksum,
                        price: bmchain.formatter.amount(1.000, 'BMT'),
                        owner: '',
                        order_id: 0,
                        apply_order: false
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

// #2: get list of encrypted contents
if (get_encrypted_content) {
    (async () => {
        try {
            const ret = await bmchain.api.getEncryptedDiscussions({
                tag: 'user001',
                owner: '',
                limit: 1,
                truncate_body: 1024
            });
            console.log(ret);
        } catch (e) {
            console.error(e);
        }
    })();
}

// #3: order to buy
if (content_order_create) {
    (async () => {
        try {
            const operations = [
                ['content_order_create',
                    {
                        author: 'user001',
                        permlink: 'encrypted-post-001-000',
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
                {active: '5KfvQrWFMifCKqPkDBaagkxK8J2JoSaPeZoKs93rrTmKgbYEh9P'}
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

// #4: get content orders
// get_content_orders user001 sviatsv -1 10
if (get_content_orders) {
    (async () => {
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
            const permlink = bmchain.formatter.commentPermlink('', 'encrypted');
            const encrypted_message = aes.encrypt(
                '5J2srqnG4cKyRdqKPZ9bwmytPmudpQthF96BUA9Sek3cseZjrPR',
                'BMT7rLYKSpuoukZ5iTraidz432uRFrGLunKP1Zy3v3cLjiGjfJPrN',
                'crypto_body',
                '111111');
            const operations = [
                ['encrypted_content',
                    {
                        parent_author: '',
                        parent_permlink: 'encrypted',
                        author: 'user001',
                        permlink: 'encrypted-post-001-001',
                        title: 'title',
                        body: 'body',
                        json_metadata: JSON.stringify({
                            tags: ['test'],
                            app: `steemjs/${pkg.version}`
                        }),
                        encrypted_message: encrypted_message.message.toString('utf8'),
                        sent_time: 111111,
                        message_size: 32,
                        checksum: encrypted_message.checksum,
                        price: bmchain.formatter.amount(1.000, 'BMT'),
                        owner: 'user002',
                        order_id: 2,
                        apply_order: true
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

// #6: get brought content and decrypt it
if (get_brought_encrypted_content){
    (async () => {
        try {
            const ret = await bmchain.api.getEncryptedDiscussions({
                tag: 'user001',
                owner: 'user002',
                limit: 1,
                truncate_body: 1024
            });
            console.log(ret);
        } catch (e) {
            console.error(e);
        }
    })();
}