var pkg = require('./package.json');
var aes = require('./steem/lib/auth/ecc/src/aes');
const bmchain = require("./steem/lib");

const username = 'user001';
const postingWif = '5JnWUyuk9jT3nW7x2pwFkz7qVLV7pgpKAXnM8dXAShwZqcXCYee';
const private_posting = '5J2srqnG4cKyRdqKPZ9bwmytPmudpQthF96BUA9Sek3cseZjrPR';
const public_posting = 'BMT7rLYKSpuoukZ5iTraidz432uRFrGLunKP1Zy3v3cLjiGjfJPrN';
const private_memo = '5J2wPb6BVAyZZYpmJHRks1DgCLEHRmzAyrwzbsvX3xaHibH8XLM';
const public_memo = 'BMT7o4Az56RoUhLay68NaUFh56sgCjStvEBt7kGzVGBqPuuQjgbpr';

// #1: posting encrypted content
(async () => {
    try
    {
        const permlink = bmchain.formatter.commentPermlink('siol', 'test');
        var message = 'enctypted message'
        const encrypted_message = aes.encrypt(private_posting, public_posting, 'enctypted message', '1');
        const operations = [
            ['encrypted_content',
                {
                    parent_author: '',
                    parent_permlink: 'test',
                    author: username,
                    permlink: permlink,
                    title: 'Test',
                    body: `This is a test using Steem.js v${pkg.version}.`,
                    json_metadata: JSON.stringify({
                        tags: ['test'],
                        app: `steemjs/${pkg.version}`
                    }),
                    encrypted_message: encrypted_message.message,
                    sent_time: 1,
                    message_size: 32,
                    checksum: encrypted_message.checksum,
                    price: '1.000 BMT',
                    owner: username,
                    order_id: 0,
                    apply_order: false
                }
            ]
        ];

        const tx = await bmchain.broadcast.sendAsync(
            { operations, extensions: [] },
            { posting: postingWif }
        );

        tx.should.have.properties([
            'expiration',
            'ref_block_num',
            'ref_block_prefix',
            'extensions',
            'operations',
            'signatures',
        ]);
    }catch (e) {
        console.error(e);
    }
})();


// #2: get list of encrypted contents

// #3: order to buy

// #4: approve order

// #5: get brought content and decrypt it