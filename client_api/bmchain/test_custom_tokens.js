var aes = require('./steem/lib/auth/ecc/src/aes');
const bmchain = require("./steem/lib");

var content_order_create = True

if (custom_token_create) {
    (async () => {
        try {
            const operations = [
                ['custom_token_create',
                    {
                        control_account: 'user001',
                        current_supply: '1000000.000 NEW',
                        custom_token_creation_fee: '1.000 BMT'
                    }
                ]
            ];

            const tx = await bmchain.broadcast.sendAsync(
                {operations, extensions: []},
                {owner: '5JnWUyuk9jT3nW7x2pwFkz7qVLV7pgpKAXnM8dXAShwZqcXCYee'}
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