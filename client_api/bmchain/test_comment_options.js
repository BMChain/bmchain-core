var pkg = require('./package.json');
const bmchain = require("./steem/lib");

(async () => {
    try
    {
        const ret = await bmchain.api.getAccounts(['initminer']);
        console.log(ret);
    }catch (e) {
        console.error(e);
    }

    if ( false ) {
        try {
            // const permlink = bmchain.formatter.commentPermlink('siol', 'test');
            const operations = [
                ['comment',
                    {
                        parent_author: '',
                        parent_permlink: 'test',
                        author: 'initminer',
                        permlink: 'post002',
                        title: 'Test',
                        body: `This is a test using Steem.js v${pkg.version}.`,
                        json_metadata: JSON.stringify({
                            tags: ['test'],
                            app: `steemjs/${pkg.version}`
                        })
                    }
                ]
            ];

            const tx = await bmchain.broadcast.sendAsync(
                {operations, extensions: []},
                {posting: '5JuL3V8GrKPV79KS7ARJUT9CzQijtx5N7f9SYDUfegizPxh9rHx'}
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
    }

    if ( true ) {
        try {
            const operations = [
                ['comment_options',
                    {
                        author: 'initminer',
                        permlink: 'post002',
                        max_accepted_payout: '1000000.000 BMT',
                        percent_steem_dollars: 10000,
                        allow_votes: true,
                        allow_curation_rewards: true,
                        "extensions":[[1,{"percent":7500}]]
                    }
                ]
            ];

            const tx = await bmchain.broadcast.sendAsync(
                {operations, extensions: []},
                {posting: '5JuL3V8GrKPV79KS7ARJUT9CzQijtx5N7f9SYDUfegizPxh9rHx'}
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
    }

    if ( true ) {
        try {
            const ret = await bmchain.api.getEncryptedDiscussions({
                tag: 'initminer',
                owner: '',
                limit: 5,
                truncate_body: 1024
            });
            console.log(ret);
        } catch (e) {
            console.error(e);
        }
    }

})();

// Name account: user001
// Private owner key  : 5J2PDGXSxrfZKvLmLvHGhSACY245gGZvbYPNXZxxwiQsdnAWVnn
// Public owner key   : BMT5hgenwjrBzeXcepgQYuQnMs1tjAfFV45778uFqpg1bHD9dfYJ6
// Private active key : 5JD4ejRcVSL5LwWDv3Sw3Do79XsQFUnp6ZJUikSUqjt45ZVfTjc
// Public active key  : BMT7aAkwAqtj4VBMDrdZDeX6BEBcX5RA6M77ixHKTtiYp8qEvXeuF
// Private posting key: 5JBxXzVd4VUnhKpPGDqwNgqAP7RMMZGifyyJKhH1zPSHepmMWH3
// Public posting key : BMT8VdgcubJKzceqhRLhqmi8eou7QPnSfhmw393PyAmGSfTofCkfh
// Private memo key   : 5JhGDZ3J8vswEChstqqRYRpcQL2PaUFUEVgXXXYM8VmYrRpDZVR
// Public memo key    : BMT621hu342tXKP5VqFwXt4wvuJjjLRxcXsMrTxthQikXoyjFP4or