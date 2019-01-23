var aes = require('./steem/lib/auth/ecc/src/aes');
const bmchain = require("./steem/lib");

var custom_token_create = false;
var get_custom_tokens = false;
var custom_token_transfer = false;
var custom_token_setup_emissions = true;

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

if (get_custom_tokens) {
    (async () => {
        try {
            const ret = await bmchain.api.getCustomTokens(10);
            console.log(ret);
        } catch (e) {
            console.error(e);
        }
    })();
}

if (custom_token_transfer) {
    (async () => {
        try {
            const operations = [
                ['custom_token_transfer',
                    {
                        from: 'user001',
                        to: 'user002',
                        amount: '1000.000 NEW'
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

if (custom_token_setup_emissions) {
    (async () => {
        try {
            const operations = [
                ['custom_token_setup_emissions',
                    {
                        schedule_time: "2019-1-1T0:0:0",
                        control_account: 'user001',

                        symbol: '0.000 NEW',
                        inflation_rate: 20,
                        interval_seconds: 60,
                        interval_count: 1
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