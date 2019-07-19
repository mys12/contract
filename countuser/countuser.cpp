#include <eosiolib/eosio.hpp>

using namespace eosio;


CONTRACT countuser: public contract {
    public:
        using contract::contract;
        ACTION countaction(name user, uint64_t count) {
            require_auth(user);
            address_index forCountuser(get_self(), get_self().value);
            auto itr = forCountuser.find(user.value);
            if( itr == forCountuser.end() ) 
            {
                forCountuser.emplace(user,[&](auto& row) {
                    row.user = user;
                    row.count = 1;
                    });
                    print("already exist");
            }
            else{
                forCountuser.modify(itr, user,[&](auto& row ) {
                    row.user = user;
                    row.count++;
                });
                print("count success");


            }
            print(itr->user, " ", itr->count);
        }
        ACTION findcount(uint64_t count) {
            
            address_index addresses(get_self(), get_self().value);
            auto forSecondary = addresses.get_index<"bycount"_n>();

            auto itr = forSecondary.require_find(count, "no count");

            print(itr->user, " ", itr->count);
        }

        ACTION eraseall() {
            require_auth(get_self());
            
            address_index forEraseAll(get_self(), get_self().value);
            auto itr = forEraseAll.begin();
            while(itr != forEraseAll.end()) { itr = forEraseAll.erase(itr); }
        }
    
    
    private:

        struct [[eosio::table]] countstruct {

        name user;
        uint64_t count;


        uint64_t primary_key() const { return user.value; }
        uint64_t by_count() const {return count;}

    };


    typedef multi_index<"counts"_n, countstruct,
    indexed_by<"bycount"_n, const_mem_fun<countstruct, uint64_t, &countstruct::by_count>> > address_index;

};

// 정답
// #include <eosio/eosio.hpp>

// using namespace eosio;

// CONTRACT countuser: public contract {
//     public:
//         using contract::contract;

//         ACTION countaction(name user) {
//             require_auth(user);

//             counts forCount(get_self(), get_self().value);
//             auto itr = forCount.find(user.value);

//             if(itr == forCount.end()) {
//                 forCount.emplace(user, [&] (auto& row) {
//                     row.user = user;
//                     row.count = 1;
//                 });
//             } else {
//                 forCount.modify(itr, user, [&] (auto& row) {
//                     row.count++;
//                 });
//             }

//             print("success");
//         }

//         ACTION findbycount(uint64_t count) {
//             counts forFind(get_self(), get_self().value);
//             auto forSecondary = forFind.get_index<"bycount"_n>();
//             auto itr = forSecondary.find(count);

//             if(itr != forSecondary.end()) {
//                 print(itr->user, " ", itr->count);
//             } else {
//                 print("nobody has that count number");
//             }
//         }

//         ACTION eraseall() {
//             require_auth(get_self());

//             counts forEraseAll(get_self(), get_self().value);
//             auto itr = forEraseAll.begin();

//             while(itr != forEraseAll.end()) {
//                 itr = forEraseAll.erase(itr);
//             }
//         }


//     private:
//         TABLE countstruct{
//             name user;
//             uint64_t count;

//             uint64_t primary_key() const { return user.value; }
//             uint64_t by_count() const { return count; }
//         };

//         typedef multi_index<"counttable"_n, countstruct, 
//         indexed_by<"bycount"_n, const_mem_fun<countstruct, uint64_t, &countstruct::by_count>> > counts;
// };