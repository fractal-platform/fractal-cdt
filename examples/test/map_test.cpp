#include <ftllib/dispatcher.hpp>
#include <ftllib/map.hpp>
#include <ftllib/crypto.hpp>

using namespace ftl;

class [[ftl::contract("test")]] test {
public:
    [[ftl::action]]
    void test1() {
        table1 t;
        t.put(1, 10);
        t.put(2, 20);
        print(t.get(1));
    }

    [[ftl::action]]
    void test2() {
        table2 t;
        t.put(1, 10);
        t.put(2, 20);
        print(t.get(1));
    }

    [[ftl::action]]
    void test3() {
        address from = get_from_address();
        checksum256 hash;
        ftl::sha256((const char*)from.addr, sizeof(from.addr), &hash);

        table3 t;
        t.put(from, hash);
        printhex(t.get(from).hash, 32);
    }

    [[ftl::action]]
    void test4() {
        table4 t;
        key_st key1, key2;
        value_st value1, value2;
        key1.i = key1.j = 1;
        value1.v = "hello";
        key2.i = key2.j = 2;
        value2.v = "hi";
        t.put(key1, value1);
        t.put(key2, value2);
        print(t.get(key2).v);
    }

    struct key_st {
        int i;
        int j;
    };

    struct value_st {
        std::string v;
    };

    DEF_TABLE(int, int, t1, table1)
    DEF_TABLE(int, uint64_t, t2, table2)
    DEF_TABLE(address, checksum256, t3, table3)
    DEF_TABLE(key_st, value_st, t4, table4)
};

FTL_DISPATCH(test, (test1)(test2)(test3)(test4))
