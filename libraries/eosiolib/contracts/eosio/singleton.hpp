#pragma once
#include "multi_index.hpp"
#include "system.hpp"

namespace  eosio {

   /**
    *  @defgroup singleton Singleton Table
    *  @ingroup contracts
    *  @brief Defines EOSIO Singleton Table used with %multiindex
    */

   /**
    *  This wrapper uses a single table to store named objects various types.
    *
    *  @ingroup singleton
    *  @tparam SingletonName - the name of this singleton variable
    *  @tparam T - the type of the singleton
    */
   template<uint64_t SingletonNameV1, uint64_t SingletonNameV2, uint64_t SingletonNameV3, uint64_t SingletonNameV4, typename T>
   class singleton
   {
      /**
       * Primary key of the data inside singleton table
       */
      constexpr static uint64_t single_name_v1 = static_cast<uint64_t>(SingletonNameV1);
      constexpr static uint64_t single_name_v2 = static_cast<uint64_t>(SingletonNameV2);
      constexpr static uint64_t single_name_v3 = static_cast<uint64_t>(SingletonNameV3);
      constexpr static uint64_t single_name_v4 = static_cast<uint64_t>(SingletonNameV4);
      constexpr static uint256_t pk_value = uint256_t(SingletonNameV1, SingletonNameV2, SingletonNameV3, SingletonNameV4);

      /**
       * Structure of data inside the singleton table
       */
      struct row {
         /**
          * Value to be stored inside the singleton table
          */
         T value;

         /**
          * Get primary key of the data
          *
          * @return uint64_t - Primary Key
          */
         uint256_t primary_key() const { return pk_value; }

         EOSLIB_SERIALIZE( row, (value) )
      };

      typedef eosio::multi_index<SingletonNameV1, SingletonNameV2, SingletonNameV3, SingletonNameV4, row> table;

      public:

         /**
          * Construct a new singleton object given the table's owner and the scope
          *
          * @param code - The table's owner
          * @param scope - The scope of the table
          */
         explicit singleton( name code, uint64_t scope ) : _t( code, scope ) {}
         explicit singleton( name code, uint128_t scope ) : _t( code, scope ) {}
         explicit singleton( name code, uint256_t scope ) : _t( code, scope ) {}
         singleton( name code, name scope ) : _t( code, scope ) {}

         /**
          *  Check if the singleton table exists
          *
          * @return true - if exists
          * @return false - otherwise
          */
         bool exists() {
            return _t.find( pk_value ) != _t.end();
         }

         /**
          * Get the value stored inside the singleton table. Will throw an exception if it doesn't exist
          *
          * @brief Get the value stored inside the singleton table
          * @return T - The value stored
          */
         T get() {
            auto itr = _t.find( pk_value );
            eosio::check( itr != _t.end(), "singleton does not exist" );
            return itr->value;
         }

         /**
          * Get the value stored inside the singleton table. If it doesn't exist, it will return the specified default value
          *
          * @param def - The default value to be returned in case the data doesn't exist
          * @return T - The value stored
          */
         T get_or_default( const T& def = T() ) {
            auto itr = _t.find( pk_value );
            return itr != _t.end() ? itr->value : def;
         }

         /**
          * Get the value stored inside the singleton table. If it doesn't exist, it will create a new one with the specified default value
          *
          * @param bill_to_account - The account to bill for the newly created data if the data doesn't exist
          * @param def - The default value to be created in case the data doesn't exist
          * @return T - The value stored
          */
         T get_or_create( name bill_to_account, const T& def = T() ) {
            auto itr = _t.find( pk_value );
            return itr != _t.end() ? itr->value
               : _t.emplace(bill_to_account, [&](row& r) { r.value = def; })->value;
         }

         /**
          * Set new value to the singleton table
          *
          * @param value - New value to be set
          * @param bill_to_account - Account to pay for the new value
          */
         void set( const T& value, name bill_to_account ) {
            auto itr = _t.find( pk_value );
            if( itr != _t.end() ) {
               _t.modify(itr, bill_to_account, [&](row& r) { r.value = value; });
            } else {
               _t.emplace(bill_to_account, [&](row& r) { r.value = value; });
            }
         }

         /**
          * Remove the only data inside singleton table
          */
         void remove( ) {
            auto itr = _t.find( pk_value );
            if( itr != _t.end() ) {
               _t.erase(itr);
            }
         }

      private:
         table _t;
   };
} /// namespace eosio
