// #include <eosio/eosio.hpp>
// #include <eosio/asset.hpp>

// using namespace eosio;

// CONTRACT scope2: public contract {
//     public:
//         using contract::contract;

//         [[eosio::on_notify("eosio.token::transfer")]]
//         void ontransfer(name from, name to, asset quantity, std::string memo) {
//             if(from == get_self()) {
//                 svreceive scpReceiver(get_self(), get_self().value);
//                 auto itr = scpReceiver.find(to.value);
//                 if (itr == scpReceiver.end()) {
//                     scpReceiver.emplace(get_self(), [&](auto& row){
//                         row.scopes = to.value;
//                     });
//                 }
                
//                 scpreceive forReceiver(get_self(), to.value);
//                 forReceiver.emplace(get_self(), [&](auto& row) {
//                     row.mykey = forReceiver.available_primary_key();
//                     row.user = from;
//                     row.balance = quantity;
//                 });
//             } else {
//                 svsend scpSender(get_self(), get_self().value);
//                 auto itr = scpSender.find(from.value);
//                 if (itr == scpSender.end()){
//                     scpSender.emplace(get_self(), [&](auto& row){
//                         row.scopes = from.value;
//                     });
//                 }
//                 send forSender(get_self(), from.value);
//                 forSender.emplace(get_self(), [&](auto& row) {
//                     row.mykey = forSender.available_primary_key();
//                     row.user = to;
//                     row.balance = quantity;
//                 });
//             }
//         }
//         ACTION eraseall {
//             require_auth(get_self());
//             while( = )
//             scpsend forEraseAll(get_self(), get_self().value);
//             auto itr = forEraseAll.begin();
//             while(itr != forEraseAll.end()) { itr = forEraseAll.erase(itr); }
//         }
//     private:
//         TABLE scope2_struct {
//             uint64_t mykey;
//             name user;
//             asset balance;

//             uint64_t primary_key() const { return mykey; }
//         };
//         TABLE scopesave_struct {
//             <uint64_t> scopes;

//             uint64_t primary_key() const { return scopes; }
//         };

//         typedef multi_index<"scopesender"_n, scope2_struct> scpsend;
//         typedef multi_index<"scopereceiver"_n, scope2_struct> scpreceive;
//         typedef multi_index<"svscpsnd"_n, scopesave_struct> svsend;
//         typedef multi_index<"svscprcv"_n, scopesave_struct> svreceive;
// };

// 정답
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT scope2: public contract {
    public:
        using contract::contract;

        [[eosio::on_notify("eosio.token::transfer")]]
        void ontransfer(name from, name to, asset quantity, std::string memo) {
            if(from == get_self()) {
                ReceiveScope forSaveScope(get_self(), get_self().value);
                auto itr = forSaveScope.find(to.value);
                if(itr == forSaveScope.end()) {
                    forSaveScope.emplace(get_self(), [&](auto& row) {
                        row.myScope = to.value;
                    });
                }

                Receive forReceiver(get_self(), to.value);
                forReceiver.emplace(get_self(), [&](auto& row) {
                    row.mykey = forReceiver.available_primary_key();
                    row.user = to;
                    row.balance = quantity;
                });
            } else {
                SendScope forSaveScope(get_self(), get_self().value);
                auto itr = forSaveScope.find(from.value);
                if(itr == forSaveScope.end()) {
                    forSaveScope.emplace(get_self(), [&](auto& row) {
                        row.myScope = from.value;
                    });
                }

                Send forSender(get_self(), from.value);
                forSender.emplace(get_self(), [&](auto& row) {
                    row.mykey = forSender.available_primary_key();
                    row.user = from;
                    row.balance = quantity;
                });
            }
        }

        ACTION eraseall() {
            require_auth(get_self());

            SendScope forSendScope(get_self(), get_self().value);
            auto itrSend = forSendScope.begin();
            while(itrSend != forSendScope.end()) {
                Send forEraseAll(get_self(), itrSend->myScope);
                auto itr = forEraseAll.begin();
                while(itr != forEraseAll.end()) {
                    itr = forEraseAll.erase(itr);
                }
                itrSend = forSendScope.erase(itrSend);
            }

            ReceiveScope forReceiveScope(get_self(), get_self().value);
            auto itrReceive = forReceiveScope.begin();
            while(itrSend != forSendScope.end()) {
                Receive forEraseAll(get_self(), itrReceive->myScope);
                auto itr = forEraseAll.begin();
                while(itr != forEraseAll.end()) {
                    itr = forEraseAll.erase(itr);
                }
                itrSend = forSendScope.erase(itrSend);
            }
        }

    private:
        TABLE tradingbook_struct {
            uint64_t mykey;
            name user;
            asset balance;

            uint64_t primary_key() const { return mykey; }
        };

        TABLE scopebook_struct {
            uint64_t myScope;

            uint64_t primary_key() const { return myScope; }
        };

        typedef multi_index<"sendbook"_n, tradingbook_struct> Send;
        typedef multi_index<"receivebook"_n, tradingbook_struct> Receive;
        typedef multi_index<"sendscope"_n, scopebook_struct> SendScope;
        typedef multi_index<"receivescope"_n, scopebook_struct> ReceiveScope;
};