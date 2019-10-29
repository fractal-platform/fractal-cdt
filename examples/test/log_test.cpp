#include <ftllib/dispatcher.hpp>
#include <ftllib/log.hpp>
#include <ftllib/address.hpp>

using namespace ftl;

class [[ftl::contract("test")]] test {
public:
    [[ftl::action]]
    void test1(std::string user) {
        print("log0: ", user);
        log(user, std::string("test"));
    }

    [[ftl::action]]
    void test2(std::string user) {
        print("log1: ", user);
        log(user, user, 20);
    }

    [[ftl::action]]
    void test3(std::string user) {
        print("log2: ", user);
        log(user, user, 20, get_from_address());
    }

};

FTL_DISPATCH(test, (test1)(test2)(test3))
