#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT transfer_contract : public contract {
   public:
      using contract::contract;

      ACTION transfer(name from, name to, asset quantity, std::string memo) {
         require_recipient(from);
         print_f("transfered : % -> % ? % (%)\n", from, to, quantity, memo);
      }
      ACTION transfer2(name from, name to, asset quantity, std::string memo) {
         require_recipient(from);
         print_f("transfered : % -> % ? % (%)\n", from, to, quantity, memo);
      }
      ACTION transfer3(name from, name to, asset quantity, std::string memo) {
         require_recipient(from);
         print_f("transfered : % -> % ? % (%)\n", from, to, quantity, memo);
      }

      using transfer_action = action_wrapper<NT(transfer), &transfer_contract::transfer>;
      using transfer2_action = action_wrapper<NT(transfer2), &transfer_contract::transfer2>;
      using transfer3_action = action_wrapper<NT(transfer3), &transfer_contract::transfer3>;
};
