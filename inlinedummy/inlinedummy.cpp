#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT inlinedummy: public contract {
    public:
        using contract::contract;

        ACTION dummy(name user) {
            require_auth(user);

            print("dummy");

            action(
                permission_level(user, "active"_n),
                get_self(),
                "dummytwo"_n,
                std::make_tuple(user)
            ).send();
        }

        ACTION dummytwo(name user) {
            require_auth(user);

            print("dummy two, ", user);
        }
    private:
};