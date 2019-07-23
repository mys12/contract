#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT scope: public contract {
    public:
        using contract::contract;

        [[eosio::on_notify("eosio.token::transfer")]]
        void ontransfer(name from, name to, asset quantity, std::string memo) {
            if(from == get_self()) {
                receive forReceiver(get_self(), to.value);
                forReceiver.emplace(get_self(), [&](auto& row) {
                    row.mykey = forReceiver.available_primary_key();
                    row.user = from;
                    row.balance = quantity;
                });
            } else {
                send forSender(get_self(), from.value);
                forSender.emplace(get_self(), [&](auto& row) {
                    row.mykey = forSender.available_primary_key();
                    row.user = to;
                    row.balance = quantity;
                });
            }
        }
    private:
        TABLE scope_struct {
            uint64_t mykey;
            name user;
            asset balance;

            uint64_t primary_key() const { return mykey; }
        };

        typedef multi_index<"scopesnd"_n, scope_struct> send;
        typedef multi_index<"scoperecv"_n, scope_struct> receive;
};