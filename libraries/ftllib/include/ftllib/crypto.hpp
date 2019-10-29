#pragma once

#include "varint.hpp"
#include "datastream.hpp"

#include <array>

namespace ftl {
    /**
     * @brief Check if the hash map the given data.
     * @param data - The source data.
     * @param hash - The hash to check.
     */
    inline void assert_sha256(const char *data, uint32_t length, const checksum256 *hash) {
        internal_use_do_not_use::assert_sha256(data, length, hash);
    }

    /**
     * @brief Get the hash of the given data.
     * @param data - The source data.
     * @param length - The length of the data.
     * @param hash - The hash result.
     */
    inline void sha256(const char *data, uint32_t length, checksum256 *hash) {
        internal_use_do_not_use::sha256(data, length, hash);
    }
} // namespace ftl


