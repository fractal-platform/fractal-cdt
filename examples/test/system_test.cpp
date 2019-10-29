#include <ftllib/dispatcher.hpp>
#include <ftllib/system.hpp>
#include <ftllib/print.hpp>

using namespace ftl;

class [[ftl::contract("test")]] test {
public:
    [[ftl::action]]
    void test1() {
        uint64_t t = current_block_time();
        print(t);
    }

    [[ftl::action]]
    void test2() {
        uint64_t h = current_block_height();
        print(h);
    }

    [[ftl::action]]
    void test3() {
        checksum256 simple_hash;
        checksum256 full_hash;
        current_block_hash(&simple_hash, &full_hash);
        printhex(simple_hash.hash, 32);
        printhex(full_hash.hash, 32);
    }
};

FTL_DISPATCH(test, (test1)(test2)(test3))
