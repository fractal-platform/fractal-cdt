#pragma once

#include "print.hpp"

namespace ftl {

#define ADDR_LEN 20

    uint8_t char2uint8(const char c) {
        if(c >= '0' && c <= '9') {
            return c - '0';
        } else if (c >= 'a' && c <= 'f') {
            return c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }
        return 0;
    }

    struct address {
        uint8_t addr[ADDR_LEN];

        address() {
            memset(addr, 0, ADDR_LEN);
        }

        address(const char* s) {
            const char *p = s + 2;    // skip 0x
            int i = 0;
            memset(addr, 0, ADDR_LEN);
            while(*p != '\0' && *(p+1) != '\0') {
                uint8_t high = char2uint8(*p);
                uint8_t low = char2uint8(*(p+1));
                addr[i] = high * 16  + low;

                i += 1;
                p += 2;
            }
        }

        constexpr bool operator==(const address &b) {
            for (auto i = 0; i < ADDR_LEN; i++) {
                if (this->addr[i] != b.addr[i])
                    return false;
            }
            return true;
        }

        constexpr bool operator!=(const address &b) {
            for (auto i = 0; i < ADDR_LEN; i++) {
                if (this->addr[i] != b.addr[i])
                    return true;
            }
            return false;
        }

    };

    template<typename DataStream>
    DataStream &operator<<(DataStream &ds, const address v) {
        for (int i = 0; i < 20; i++) {
            ds << v.addr[i];
        }
        return ds;
    }

    template<typename DataStream>
    DataStream &operator>>(DataStream &ds, address &v) {
        for (int i = 0; i < 20; i++) {
            ds >> v.addr[i];
        }
        return ds;
    }

    /**
        * @brief Get the address of the user who call contracts.
        * @return The address of the user who call contracts.
        * @retval address
        */
    address get_from_address() {
        address addr;
        internal_use_do_not_use::get_from(addr.addr, ADDR_LEN);

        return addr;
    }

    /**
       * @brief Get the address the contract the user is calling.
       * @return The address of the contract the user is calling.
       * @retval address
       */
    address get_to_address() {
        address addr;
        internal_use_do_not_use::get_to(addr.addr, ADDR_LEN);

        return addr;
    }

    /**
      * @brief Get the owner's address of the contract.
      * @return The owner's address of the contract.
      * @retval address
      */
    address get_owner_address() {
        address addr;
        internal_use_do_not_use::get_owner(addr.addr, ADDR_LEN);

        return addr;
    }

    /**
     * @brief Print an address.
     * @param addr - An address
     *
     */
    void print_address(address addr) {
        ftl::printhex(addr.addr, ADDR_LEN);
    }

}
