#include <appbase/application.hpp>
#include <steem/http_plugin/http_plugin.hpp>
#include <steem/net_plugin/net_plugin.hpp>
#include <fc/log/logger_config.hpp>

using namespace appbase;
using namespace steem;

int main( int argc, char** argv ) 
{
   fc::set_thread_name( "main" );
   try {
      app().register_plugin<net_plugin>();
      app().register_plugin<http_plugin>();

      if( !app().initialize( argc, argv ) )
         return -1;

      appbase::app().startup();
      appbase::app().exec();

   } catch ( const fc::exception& e ) {
      elog( "${e}", ("e",e.to_detail_string() ) );
   } catch ( const boost::exception& e ) {
      elog( "${e}", ("e",boost::diagnostic_information(e))  );
   } catch ( const std::exception& e ) {
      elog( "${e}", ("e",e.what() ) );
   } catch ( ... ) {
      elog( "unknown exception" );
   }
   return 0;
}
