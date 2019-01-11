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
const wif = '5JnWUyuk9jT3nW7x2pwFkz7qVLV7pgpKAXnM8dXAShwZqcXCYee';

bmchain.broadcast.encrypted_content(
        wif,
        username,
        '',
        '',
        username,
        'encrypted_post_000',
        'encrypted_post_title',
        'encrypted_post_body',
        'json_metadata',
        'encrypted_message',
        1,
        2,
        3,
        '1.000 BMT',
        username,
        0,
        1,
        function(err, result) {
            console.log(err, result);
        }
    );

bmchain.broadcast.encrypted_content(
    wif,
    username,
    '',
    '',
    username,
    'encrypted_post_001',
    'encrypted_post_title',
    'encrypted_post_body',
    'json_metadata',
    'encrypted_message',
    1,
    2,
    3,
    '0.000 BMT',
    username,
    0,
    1,
    function(err, result) {
        console.log(err, result);
    }
);