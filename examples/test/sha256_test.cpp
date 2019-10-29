#include <ftllib/dispatcher.hpp>
#include <ftllib/crypto.hpp>

using namespace ftl;

class [[ftl::contract("test")]] test {
public:
    [[ftl::action]]
    void test1() {
        char test[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
        checksum256 hash;
        ftl::sha256(test, sizeof(test), &hash);
        printhex(hash.hash, 32);
        //hash.hash[0] = 0;
        ftl::assert_sha256(test, sizeof(test), &hash);
    }
};

FTL_DISPATCH(test, (test1))
