#include <eosiolib/eosio.hpp>

using namespace eosio;

CONTRACT example: public contract {
    public: 
        
        using contract::contract;
            
            ACTION hi(name user) {
             bool isTrue = has_auth(get_self());
             check(isTrue, "you aint a contract deployer!!");
             
             address_index forHi(get_self(), get_self().value);
             auto itr = forHi.find(user.value);
             check(itr == forHi.end(), "already done");
             print("hello, ", user);
            }
            
            ACTION insert(name user){
                bool isTrue = has_auth(get_self());

                check(isTrue, "no insert");


                address_index forInsert(get_self(), get_self().value);

                auto itr = forInsert.find(user.value);


                check(itr == forInsert.end(), "already exists");


                forInsert.emplace(get_self(), [&](auto& row) {

                    row.user = user;

                });


                print("insert success");


            }

            ACTION remove(name user) {

                bool isTrue = has_auth(get_self());

                check(isTrue, "no remove");


                address_index forRemove(get_self(), get_self().value);

                auto itr = forRemove.require_find(user.value, "no account");

                forRemove.erase(itr);


                print("remove success");

            }



    private :
    struct [[eosio::table]] allowance {

        name user = user;


        uint64_t primary_key() const { return user.value; }

    };
    typedef multi_index<"allowances"_n, allowance> address_index;


};