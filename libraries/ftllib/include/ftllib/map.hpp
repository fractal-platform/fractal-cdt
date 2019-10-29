#pragma once

#include "name.hpp"
#include "datastream.hpp"

#include <vector>
#include <tuple>
#include <functional>
#include <utility>
#include <limits>
#include <algorithm>
#include <memory>
#include <cstring>

namespace ftl {

#define DEF_TABLE(tbl_key, tbl_value, tbl_name, tbl_typename) \
    struct [[ftl::table]] tbl_name { \
        tbl_key key; \
        tbl_value value; \
    }; \
    typedef table<ftl::name(#tbl_name), tbl_key, tbl_value> tbl_typename;

#define MAX_KEY_LENGTH 32

    class MapKey {
    public:
        uint8_t length;
        uint8_t bytes[MAX_KEY_LENGTH];

        MapKey() {
            this->length = 0;
            memset(this->bytes, 0, MAX_KEY_LENGTH);
        }

        template<typename T>
        MapKey(T t) {
            size_t size = ftl::pack_size(t);
            check(size <= MAX_KEY_LENGTH, "key size must be smaller than 32");

            ftl::datastream<char *> ds((char *) this->bytes, size);
            ds << t;

            this->length = size;
        }
    };

    template<typename DataStream>
    DataStream &operator<<(DataStream &ds, const MapKey &key) {
        ds << key.length;
        for (int i = 0; i < key.length; i++) {
            ds << key.bytes[i];
        }
        return ds;
    }

    template<typename DataStream>
    DataStream &operator>>(DataStream &ds, MapKey &key) {
        ds >> key.length;
        for (int i = 0; i < key.length; i++) {
            ds >> key.bytes[i];
        }
        return ds;
    }

    void db_store(uint64_t table, const MapKey &key, const void *buffer, size_t buffer_size) {
        internal_use_do_not_use::db_store(table, key.bytes, key.length, buffer, buffer_size);
    }

    int db_load(uint64_t table, const MapKey &key, void *buffer, size_t buffer_size) {
        return internal_use_do_not_use::db_load(table, key.bytes, key.length, buffer, buffer_size);
    }

    int db_has_key(uint64_t table, const MapKey &key) {
        return internal_use_do_not_use::db_has_key(table, key.bytes, key.length);
    }

    void db_remove_key(uint64_t table, const MapKey &key) {
        return internal_use_do_not_use::db_remove_key(table, key.bytes, key.length);
    }

    template<ftl::name::raw TableName, typename KT, typename VT>
    class table {
    private:

        constexpr static bool validate_table_name(ftl::name n) {
            // Limit table names to 12 characters so that the last character (4 bits) can be used to distinguish between the secondary indices.
            return n.length() < 13; //(n & 0x000000000000000FULL) == 0;
        }

        constexpr static size_t max_stack_buffer_size = 512;

        static_assert(validate_table_name(ftl::name(TableName)),
                      "table does not support table names with a length greater than 12");

    public:
        table() {}

        void put(const KT &key, const VT &value) {
            size_t size = ftl::pack_size(value);

            //using malloc/free here potentially is not exception-safe, although WASM doesn't support exceptions
            void *buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
            ftl::datastream<char *> ds((char *) buffer, size);
            ds << value;

            auto pk = MapKey(key);
            db_store(static_cast<uint64_t>(TableName), pk, buffer, size);

            if (max_stack_buffer_size < size) {
                free(buffer);
            }
        }

        const VT get(const KT &key) const {
            MapKey primary(key);
            auto size = db_load(static_cast<uint64_t>(TableName), primary, nullptr, 0);
            ftl::check(size >= 0, "error get from primary key");

            //using malloc/free here potentially is not exception-safe, although WASM doesn't support exceptions
            void *buffer = max_stack_buffer_size < size_t(size) ? malloc(size_t(size)) : alloca(size_t(size));
            db_load(static_cast<uint64_t>(TableName), primary, buffer, uint32_t(size));

            VT obj;
            ftl::datastream<const char *> ds((char *) buffer, uint32_t(size));
            ds >> obj;

            if (max_stack_buffer_size < size_t(size)) {
                free(buffer);
            }

            return static_cast<const VT>(obj);
        }

        bool has_key(const KT &key) const {
            return db_has_key(static_cast<uint64_t>(TableName), MapKey(key));
        }

        void erase(const KT &key) {
            db_remove_key(static_cast<uint64_t>(TableName), MapKey(key));
        }

    };
}  /// ftl
