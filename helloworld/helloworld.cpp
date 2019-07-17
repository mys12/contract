#include <eosiolib/eosio.hpp>

using namespace eosio;

CONTRACT helloworld: public contract {
    public: 
        using contract::contract;
        ACTION hi(name user) {
            check(has_auth(user), "missing authority"); // has_auth() -> true or false 
            
            // == require_auth(user);
            
            
            print("hello, ", user);
        }
    private :


};