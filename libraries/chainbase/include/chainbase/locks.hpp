#pragma once

#include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <boost/interprocess/exceptions.hpp>

#include <atomic>

namespace chainbase 
{
   namespace bip = boost::interprocess;

   /**
    * A recursive sharable mutex using boost interprocess.
    *
    * The interprocess sharable mutex had some undefined behavior we were running
    * into where two readers would be stuck acquiring shared access at the same
    * time as the writer would be stick releasing exclusive access.
    *
    * This is a naive but well defined implementation of the same lock structure.
    * It includes a recursive exclusive mutex because steemd was not designed with
    * parallel access in mind and can sometimes recursively lock. It could be
    * refactored to eliminate this behavior, but this was the quickest and easiest
    * solution.
    *
    * This mutex conforms to the same interface as the boost interprocess_sharable_mutex
    * and can be used with the boost sharable_lock and scoped_lock.
    */
   struct interprocess_recursive_sharable_mutex
   {
      interprocess_recursive_sharable_mutex() {}

      ~interprocess_recursive_sharable_mutex() {}

      void lock()
      {
         _write_mutex.lock();

         bip::interprocess_mutex cond_mutex;
         bip::scoped_lock< bip::interprocess_mutex > cond_lock( cond_mutex );

         while( _readers.load() > 0 )
            _read_cond.wait( cond_lock );
      }

      bool try_lock()
      {
         bool result = _write_mutex.try_lock();

         if( result && _readers.load() > 0 )
         {
            _write_mutex.unlock();
            result = false;
         }

         return result;
      }

      bool timed_lock( const boost::posix_time::ptime& abs_time )
      {
         bool result = _write_mutex.timed_lock( abs_time );

         if( !result )
            return result;

         bip::interprocess_mutex cond_mutex;
         bip::scoped_lock< bip::interprocess_mutex > cond_lock( cond_mutex );
         result = cond_lock.owns();

         if( !result )
            return result;

         while( result && _readers.load() > 0 )
            result = _read_cond.timed_wait( cond_lock, abs_time );

         if( !result )
            _write_mutex.unlock();

         return result;
      }

      void unlock()
      {
         _write_mutex.unlock();
      }

      void lock_sharable()
      {
         _write_mutex.lock();

         if( _readers.load() == ~0 )
         {
            _write_mutex.unlock();
            throw bip::interprocess_exception( "Unable to acquire shared lock" );
         }

         _readers++;
         _write_mutex.unlock();
      }

      bool try_lock_sharable()
      {
         bool result = _write_mutex.try_lock();

         if( result )
         {
            if( _readers.load() == ~0 )
               return false;
            else
               _readers++;
            
            _write_mutex.unlock();
         }

         return result;
      }

      bool timed_lock_sharable( const boost::posix_time::ptime& abs_time )
      {
         bool result = _write_mutex.timed_lock( abs_time );

         if( result )
         {
            if( _readers.load() == ~0 )
               result = false;
            else
               _readers++;
            
            _write_mutex.unlock();
         }

         return result;
      }

      void unlock_sharable()
      {
         _readers--;
         if( _readers.load() == 0 )
            _read_cond.notify_all();
      }

      
      std::atomic< uint32_t >           _readers;
      bip::interprocess_recursive_mutex _write_mutex;
      bip::interprocess_condition       _read_cond;
   };

   typedef interprocess_recursive_sharable_mutex read_write_mutex;
   typedef bip::sharable_lock< read_write_mutex > read_lock;
   typedef bip::scoped_lock< read_write_mutex > write_lock;
}