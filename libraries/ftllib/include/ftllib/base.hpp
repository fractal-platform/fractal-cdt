#pragma once

#include <cstring>

namespace ftl {

    constexpr static size_t max_stack_buffer_size = 512;

    struct __attribute__((aligned (16))) checksum256 {
        uint8_t hash[32];

        checksum256() {
            memset(hash, 0, 32);
        }
    };

    template<typename DataStream>
    DataStream &operator<<(DataStream &ds, const checksum256 v) {
        for (int i = 0; i < 32; i++) {
            ds << v.hash[i];
        }
        return ds;
    }

    template<typename DataStream>
    DataStream &operator>>(DataStream &ds, checksum256 &v) {
        for (int i = 0; i < 32; i++) {
            ds >> v.hash[i];
        }
        return ds;
    }

    namespace internal_use_do_not_use {
        extern "C" {
        __attribute__((ftl_wasm_import))
        void assert_sha256(const char *data, uint32_t length, const checksum256 *hash);

        __attribute__((ftl_wasm_import))
        void sha256(const char *data, uint32_t length, checksum256 *hash);

        __attribute__((ftl_wasm_import))
        uint32_t read_action_data(void *msg, uint32_t len);

        __attribute__((ftl_wasm_import))
        uint32_t action_data_size();

        __attribute__((ftl_wasm_import))
        void transfer(void *addr, size_t addr_size, uint64_t amount);

        __attribute__((ftl_wasm_import))
        uint64_t get_amount();

        __attribute__((ftl_wasm_import))
        int call_action(void *addr, size_t addr_size, void *action, size_t action_size,
                        uint64_t amount, int storage_delegate, int user_delegate);

        __attribute__((ftl_wasm_import))
        size_t call_result(void *result, size_t result_size);

        __attribute__((ftl_wasm_import))
        void get_from(void *buffer, size_t buffer_size);

        __attribute__((ftl_wasm_import))
        void get_to(void *buffer, size_t buffer_size);

        __attribute__((ftl_wasm_import))
        void get_owner(void *buffer, size_t buffer_size);

        __attribute__((ftl_wasm_import))
        void ftl_assert(uint32_t test, const char *msg);

        __attribute__((ftl_wasm_import))
        void ftl_assert_message(uint32_t test, const char *msg, uint32_t msg_len);

        __attribute__((ftl_wasm_import))
        void ftl_assert_code(uint32_t test, uint64_t code);

        __attribute__((ftl_wasm_import))
        size_t set_result(void *result, size_t result_size);

        __attribute__((ftl_wasm_import))
        void log_0(const void *data, size_t data_size, const checksum256 *name);

        __attribute__((ftl_wasm_import))
        void log_1(const void *data, size_t data_size, const checksum256 *name, const checksum256 *param1);

        __attribute__((ftl_wasm_import))
        void log_2(const void *data, size_t data_size, const checksum256 *name, const checksum256 *param1,
                   const checksum256 *param2);

        __attribute__((ftl_wasm_import))
        void db_store(uint64_t table, const void *key, size_t key_size, const void *buffer, size_t buffer_size);

        __attribute__((ftl_wasm_import))
        int db_load(uint64_t table, const void *key, size_t key_size, void *buffer, size_t buffer_size);

        __attribute__((ftl_wasm_import))
        int db_has_key(uint64_t table, const void *key, size_t key_size);

        __attribute__((ftl_wasm_import))
        void db_remove_key(uint64_t table, const void *key, size_t key_size);

        __attribute__((ftl_wasm_import))
        int db_has_table(uint64_t table);

        __attribute__((ftl_wasm_import))
        void db_remove_table(uint64_t table);

        __attribute__((ftl_wasm_import))
        void printn(uint64_t);

        __attribute__((ftl_wasm_import))
        void prints(const char *);

        __attribute__((ftl_wasm_import))
        void prints_l(const char *, uint32_t);

        __attribute__((ftl_wasm_import))
        void printi(int64_t);

        __attribute__((ftl_wasm_import))
        void printui(uint64_t);

        __attribute__((ftl_wasm_import))
        void printi128(const int128_t *);

        __attribute__((ftl_wasm_import))
        void printui128(const uint128_t *);

        __attribute__((ftl_wasm_import))
        void printsf(float);

        __attribute__((ftl_wasm_import))
        void printdf(double);

        __attribute__((ftl_wasm_import))
        void printqf(const long double *);

        __attribute__((ftl_wasm_import))
        void printn(uint64_t);

        __attribute__((ftl_wasm_import))
        void printhex(const void *, uint32_t);

        __attribute__((ftl_wasm_import, noreturn))
        void ftl_exit(int32_t code);

        __attribute__((ftl_wasm_import))
        uint64_t current_time();

        __attribute__((ftl_wasm_import))
        uint64_t current_height();

        __attribute__((ftl_wasm_import))
        void current_hash(checksum256 *simple_hash, checksum256 *full_hash);
        }
    };
}
