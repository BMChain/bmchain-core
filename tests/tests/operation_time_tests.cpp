#ifdef IS_TEST_NET
#include <boost/test/unit_test.hpp>

#include <bmchain/protocol/exceptions.hpp>

#include <bmchain/chain/block_summary_object.hpp>
#include <bmchain/chain/database.hpp>
#include <bmchain/chain/hardfork.hpp>
#include <bmchain/chain/history_object.hpp>
#include <bmchain/chain/bmchain_objects.hpp>

#include <bmchain/chain/util/reward.hpp>

#include <bmchain/plugins/debug_node/debug_node_plugin.hpp>

#include <fc/crypto/digest.hpp>

#include "../common/database_fixture.hpp"

#include <cmath>

using namespace bmchain;
using namespace bmchain::chain;
using namespace bmchain::protocol;

BOOST_FIXTURE_TEST_SUITE( operation_time_tests, clean_database_fixture )

BOOST_AUTO_TEST_SUITE_END()
#endif
