var pkg = require('./package.json');
const bmchain = require("./steem/lib");

(async () => {
    try
    {
        const ret = await bmchain.api.getAccounts(['user001']);
        console.log(ret);
    }catch (e) {
        console.error(e);
    }
})();

const username = 'user001';
const postingWif = '5JnWUyuk9jT3nW7x2pwFkz7qVLV7pgpKAXnM8dXAShwZqcXCYee';

(async () => {
    try
    {
        const permlink = bmchain.formatter.commentPermlink('siol', 'test');
        const operations = [
            ['comment',
                {
                    parent_author: '',
                    parent_permlink: 'test',
                    author: username,
                    permlink,
                    title: 'Test',
                    body: `This is a test using Steem.js v${pkg.version}.`,
                    json_metadata : JSON.stringify({
                        tags: ['test'],
                        app: `steemjs/${pkg.version}`
                    })
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

// #1: posting encrypted content

// #2: get list of encrypted contents

// #3: order to buy

// #4: approve order

// #5: get brought content and decrypt it