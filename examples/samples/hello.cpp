#include <ftllib/dispatcher.hpp>

using namespace ftl;

class [[ftl::contract("hello")]] hello {
public:
    [[ftl::action]]
    void hi() {
        printf("Hello World");
    }
};

FTL_DISPATCH(hello, (hi))
