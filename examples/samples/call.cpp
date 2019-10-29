#include <cstring>
#include <ftllib/dispatcher.hpp>
#include <ftllib/check.hpp>
#include <ftllib/map.hpp>
#include <ftllib/address.hpp>

using namespace ftl;

struct person {
    std::string first_name;
    std::string last_name;
    std::string city;
};

extern std::string insert(std::string first_name, std::string last_name, std::string city);
extern struct person modify(std::string first_name, std::string last_name, std::string city);
extern void erase();
const address addressbook_contract("0x44B70EF70DD7b0e7Cf0Ae5f243c0d422017926cb");

class [[ftl::contract("call")]] call{

public:

    [[ftl::action]]
    void test1(std::string first_name, std::string last_name, std::string city) {
        std::string ret;
        CALL(&ret, addressbook_contract, "insert", insert, 0, first_name, last_name, city);
        print_f("call insert ok: %, %, %, %", first_name, last_name, city, ret);
        //print("call ok");
    }

    [[ftl::action]]
    void test2(std::string first_name, std::string last_name, std::string city) {
        std::string ret;
        DELEGATE_CALL(&ret, addressbook_contract, "insert", insert, 0, first_name, last_name, city);
        print_f("delegate insert ok: %, %, %, %", first_name, last_name, city, ret);
        //print("delegate call ok");
    }

    [[ftl::action]]
    void test3(std::string first_name, std::string last_name, std::string city) {
        struct person p;
        CALL(&p, addressbook_contract, "modify", modify, 0, first_name, last_name, city);
        print_f("call modify ok: %, %, %, %", first_name, last_name, city, p.first_name);
        //print("call modify ok");
    }

    [[ftl::action]]
    void test4() {
        CALL_NOARGS(NULL, addressbook_contract, "erase", erase, 0);
        print_f("call erase ok");
    }
};

FTL_DISPATCH( call, (test1)(test2)(test3)(test4) )