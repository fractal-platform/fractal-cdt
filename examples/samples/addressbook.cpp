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

class [[ftl::contract("addressbook")]] addressbook {

public:
    [[ftl::action]]
    std::string insert(std::string first_name, std::string last_name, std::string city) {
        ptable t;
        address addr = get_from_address();
        check(!t.has_key(addr), "already exist");
        person p;
        p.first_name = first_name;
        p.last_name = last_name;
        p.city = city;
        t.put(addr, p);
        print_f("insert ok: %, %, %", first_name, last_name, city);
        return first_name;
    }

    [[ftl::action]]
    struct person modify(std::string first_name, std::string last_name, std::string city) {
        ptable t;
        address addr = get_from_address();
        check(t.has_key(addr), "not exist");
        person p;
        p.first_name = first_name;
        p.last_name = last_name;
        p.city = city;
        t.put(addr, p);
        print_f("modify ok: %, %, %", first_name, last_name, city);
        return p;
    }

    [[ftl::action]]
    void erase() {
        ptable t;
        address addr = get_from_address();
        check(t.has_key(addr), "not exist");
        t.erase(addr);
        print_f("erase ok");
    }

private:
    DEF_TABLE(address, person, p, ptable)
};

FTL_DISPATCH( addressbook, (insert)(modify)(erase) )