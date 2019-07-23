// #include <eosio/eosio.hpp>
// #include <eosio/asset.hpp>

// using namespace eosio;

// CONTRACT available: public contract {
//     public:
//         using contract::contract;

//         ACTION dummy() {}


//         [[eosio::on_notify("eosio.token::transfer")]]
//         void ontransfer(name from, name to, asset quantity, std::string memo) {
            
//             if(from == get_self()) {
//                 Arecver receiver(get_self(), get_self().value);
                
//                 receiver.emplace(from, [&](auto& row) {
//                     row.mykey = receiver.available_primary_key();
//                     row.user = to;
//                     row.balance = quantity;
                    
//                 });

//             } else {
//                 Asender sender(get_self(), get_self().value);
                
                
//                 sender.emplace(to, [&](auto& row) {
//                     row.mykey = sender.available_primary_key();
//                     row.user = from;
//                     row.balance = quantity;
                    
//                 });
//             }
//         }
        

//     private:
//         TABLE available_struct {
//             uint64_t mykey;
//             name user;
//             asset balance;
            

//             uint64_t primary_key() const { return mykey; }
//         };

//         typedef multi_index<"avasend"_n, available_struct> Asender;
//         typedef multi_index<"avarecv"_n, available_struct> Arecver;
// };


// 정답 
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
                Receive forReceiver(get_self(), get_self().value);
                forReceiver.emplace(get_self(), [&](auto& row) {
                    row.mykey = forReceiver.available_primary_key();
                    row.user = from;
                    row.balance = quantity;
                });
            } else {
                Send forSender(get_self(), get_self().value);
                forSender.emplace(get_self(), [&](auto& row) {
                    row.mykey = forSender.available_primary_key();
                    row.user = to;
                    row.balance = quantity;
                });
            }
        }
    private:
        TABLE tradingbook_struct {
            uint64_t mykey;
            name user;
            asset balance;

            uint64_t primary_key() const { return mykey; }
        };

        typedef multi_index<"senderbook"_n, tradingbook_struct> Send;
        typedef multi_index<"receiverbook"_n, tradingbook_struct> Receive;
};