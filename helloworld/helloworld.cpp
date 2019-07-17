#include <eosiolib/eosio.hpp>

using namespace eosio;

CONTRACT helloworld: public contract {
    public: 
        
        using contract::contract;
        ACTION hi(name user) {
            check(has_auth("moonmoon1234"_n), "missing authority"); // has_auth() -> true or false 
            
            // == require_auth(user);
            // require_auth("moonmoon1234"_n);
            // require_auth(get_self()); -> 동적으로,, 할당해서 제일 추천
            // check(has_auth(get_self(), "missing authority"));
            
            print("hello, ", user);
        }
    private :


};