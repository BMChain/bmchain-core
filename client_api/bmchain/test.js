const bmchain = require("./steem/lib");

(async () => {
    try
    {
        const ret = await bmchain.api.getAccounts(['bmchain']);
        console.log(ret);
    }catch (e) {
        console.error(e);
    }
})();