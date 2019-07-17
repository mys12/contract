#include <eosiolib/eosio.hpp>

using namespace eosio;

CONTRACT helloworld: public contract {
    public: 
        
        using contract::contract;
        ACTION hi(name user) {
            // bool isTrue = has_auth(get_self()) || has_auth("hahahoho1234"_n);
            // check(isTrue, "you aint a contract deployer!!")
            check(has_auth(get_self()) || has_auth("hahahoho1234"_n),"missing authority");
            //check(has_auth("moonmoon1234"_n), "missing authority"); // has_auth() -> true or false 
            
            // == require_auth(user);
            // require_auth("moonmoon1234"_n);
            // require_auth(get_self()); -> 동적으로,, 할당해서 제일 추천
            // check(has_auth(get_self(), "missing authority"));
            
            print("hello, ", user);
        }
    private :


};