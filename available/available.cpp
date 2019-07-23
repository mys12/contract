#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT available: public contract {
    public:
        using contract::contract;

        ACTION dummy() {}


        [[eosio::on_notify("eosio.token::transfer")]]
        void ontransfer(name from, name to, asset quantity, std::string memo) {
            
            if(from == get_self()) {
                Arecver receiver(get_self(), get_self().value);
                auto itr = receiver.find(to.value);
                
                if(itr == receiver.end()) {
                    receiver.emplace(from, [&](auto& row) {
                        row.user = to;
                        row.balance = quantity;
                        row.mykey = receiver.available_primary_key();
                    });
                }
            } else {
                Asender sender(get_self(), get_self().value);
                auto itr = sender.find(from.value);
                
                if(itr == sender.end()) {
                    sender.emplace(to, [&](auto& row) {
                        row.user = from;
                        row.balance = quantity;
                        row.mykey = sender.available_primary_key();
                    });
                }
            }
        }

    private:
        TABLE available_struct {
            name user;
            asset balance;
            uint64_t mykey;

            uint64_t primary_key() const { return mykey; }
        };

        typedef multi_index<"avasend"_n, available_struct> Asender;
        typedef multi_index<"avarecv"_n, available_struct> Arecver;
};