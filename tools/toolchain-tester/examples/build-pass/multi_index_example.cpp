#include <eosio/eosio.hpp>
using namespace eosio;

CONTRACT multi_index_example : public contract {
   public:
      using contract::contract;
      multi_index_example( name receiver, name code, datastream<const char*> ds )
         : contract(receiver, code, ds), testtab(receiver, receiver.value) {}

      ACTION set(name user);
      ACTION print( name user );
      ACTION bysec( name secid );
      ACTION mod( name user, uint32_t n );

      TABLE test_table {
         name test_primary;
         name secondary;
         uint64_t datum;
         uint64_t primary_key()const { return test_primary.value; }
         uint64_t by_secondary()const { return secondary.value; }
      };

      typedef eosio::multi_index<NT(testtaba), test_table, eosio::indexed_by<NT(secid), eosio::const_mem_fun<test_table, uint64_t, &test_table::by_secondary>>> test_tables;

      using set_action = action_wrapper<NT(set), &multi_index_example::set>;
      using print_action = action_wrapper<NT(print), &multi_index_example::print>;
      using bysec_action = action_wrapper<NT(bysec), &multi_index_example::bysec>;
      using mod_action = action_wrapper<NT(mod), &multi_index_example::mod>;
      test_tables testtab;
};

ACTION multi_index_example::set( name user ) {
   auto itr = testtab.find(user.value);
   if ( itr == testtab.end() ) {
      testtab.emplace( _self, [&]( auto& u ) {
            u.test_primary = user;
            u.secondary = "second"_n;
            u.datum = 0;
      });
   }
}

ACTION multi_index_example::print( name user ) {
   auto itr = testtab.find(user.value);
   check( itr != testtab.end(), "test table not set" );
   eosio::print_f("Test Table : {%, %, %}\n", itr->test_primary, itr->secondary, itr->datum);
}

ACTION multi_index_example::bysec( name secid ) {
   auto idx = testtab.get_index<"secid"_n>();
   for ( auto itr = idx.begin(); itr != idx.end(); itr++ ) {
      print( itr->test_primary );
   }
}


ACTION multi_index_example::mod( name user, uint32_t n ) {
   auto itr = testtab.find(user.value);
   check( itr != testtab.end(), "test table not set" );
   testtab.modify( itr, _self, [&]( auto& row ) {
      row.secondary = user;
      row.datum = n;
   });
}
