#pragma once

#include "crypto.hpp"
#include "datastream.hpp"

namespace ftl {

    template<typename T>
    checksum256 *log_hash(T arg) {
        size_t size = ftl::pack_size(arg);
        void *buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
        ftl::datastream<char *> ds((char *) buffer, size);
        ds << arg;

        checksum256 *hash = (checksum256 *) malloc(sizeof(checksum256));
        sha256((char *) buffer, size, hash);
        if(max_stack_buffer_size < size) {
            free(buffer);
        }
        return hash;
    }

    template<typename ...T>
    constexpr bool is_pointer_typename() {
        bool arr[] = {std::is_pointer<T>()...};
        int count = sizeof...(T);
        bool ret = false;
        for(int i = 0; i < count; i++) {
            ret = ret || arr[i];
        }
        return ret;
    }

    /**
     * @brief Write a log to node.
     * @param name - A string you want to log.
     * @param args - The amount of the args is 3.
     *
     */
    template<typename T1, typename ...T2, typename std::enable_if_t<!std::is_pointer<T1>::value && !is_pointer_typename<T2...>()> * = nullptr>
    void log(const std::string name, T1 data, T2... args) {
        size_t size = ftl::pack_size(data);
        void *buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
        ftl::datastream<char *> ds((char *) buffer, size);
        ds << data;

        checksum256 *hash_name = log_hash(name);
        int num = sizeof...(args);
        checksum256 *hash_list[] = {log_hash(args)...};
        if (num == 0) {
            internal_use_do_not_use::log_0(buffer, size, hash_name);
        } else if (num == 1) {
            internal_use_do_not_use::log_1(buffer, size, hash_name, hash_list[0]);
        } else if (num == 2) {
            internal_use_do_not_use::log_2(buffer, size, hash_name, hash_list[0], hash_list[1]);
        }

        free(buffer);
        free(hash_name);
        for (int i = 0; i < num; i++) {
            free(hash_list[i]);
        }
    }

}
