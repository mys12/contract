#include <eosio/eosio.hpp>

using namespace eosio;

CONTRACT addressbook: public contract {
    public:
        using contract::contract;
        
        ACTION findage(uint64_t age) {
            address_index addresses(get_self(), get_self().value);
            auto forSecondary = addresses.get_index<"byage"_n>();

            auto itr = forSecondary.require_find(age, "no age");

            print(itr->user, " ", itr->age);
        }

        ACTION upsert(name user, uint64_t age) {
            require_auth(user);
            address_index forUpsert(get_self(), get_self().value);
            auto itr = forUpsert.find(user.value);

            if (itr == forUpsert.end()) {
                forUpsert.emplace(user, [&](auto& row){
                    row.user = user;
                    row.age = age; 
                });
                print("insert success");
            } else {
                forUpsert.modify(itr, user, [&](auto& row) {
                    row.user = user;
                    row.age = age;
                });
                print("modify success");
            } 
            
        }
        
        ACTION insert(name user, uint64_t age) {
            require_auth(user);
            address_index forInsert(get_self(), get_self().value);
            auto itr = forInsert.find(user.value);

            check(itr == forInsert.end(), "already exists");

            forInsert.emplace(user, [&](auto& row){   
                row.user = user;
                row.age = age; 
            });
            print("insert success");
        }

        ACTION erase(name user) {  
            require_auth(user);     

            address_index forErase(get_self(), get_self().value); 
            auto itr = forErase.require_find(user.value, "no account"); 
            forErase.erase(itr);
            print("erase success");
        }

    private:
        TABLE person {
            name user;
            uint64_t age;

            uint64_t primary_key() const { return user.value; }     
            uint64_t by_age() const { return age; }
        };
        typedef multi_index<"peopletwo"_n, person, indexed_by<"byage"_n, const_mem_fun<person, uint64_t, &person::by_age>> >address_index;      // multi_index<>를 address_index로 간단하게 변환
};


