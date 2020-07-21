#include <eosio/eosio.hpp>
using namespace eosio;

CONTRACT hello : public contract {
   public:
      using contract::contract;

      ACTION hi( name nm );
      ACTION check( name nm );

      using hi_action = action_wrapper<NT(hi), &hello::hi>;
      using check_action = action_wrapper<NT(check), &hello::check>;
};

ACTION hello::hi( name nm ) {
   print_f("Name : %\n", nm);
}

ACTION hello::check( name nm ) {
   print_f("Name : %\n", nm);
   eosio::check(nm == "hello"_n, "check name not equal to `hello`");
}
