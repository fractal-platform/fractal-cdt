#pragma once

#include "crypto.hpp"
#include "check.hpp"

namespace ftl {
    /**
     *  @addtogroup system System
     *  @ingroup contracts
     *  @brief Defines time related functions and ftl_exit
     */

    /**
     *  This method will abort execution of wasm without failing the contract. This is used to bypass all cleanup / destructors that would normally be called.
     *
     *  @ingroup system
     *  @param code - the exit code
     *  Example:
     *
     *  @code
     *  ftl_exit(0);
     *  ftl_exit(1);
     *  @endcode
     */
    inline void ftl_exit(int32_t code) {
        internal_use_do_not_use::ftl_exit(code);
    }

    /**
    *  Returns the time in seconds from 1970 of the current block
    *
    *  @ingroup system
    *  @return time in seconds from 1970 of the current block
    */
    inline uint64_t current_block_time() {
        return internal_use_do_not_use::current_time();
    }

    /**
     * Returns the height of the current block
     *
     * @ingroup system
     * @return current block height
     */
    inline uint64_t current_block_height() {
        return internal_use_do_not_use::current_height();
    }

    /**
     *
     * @ingroup system
     * @param simple_hash
     * @param full_hash
     */
    inline void current_block_hash(checksum256* simple_hash, checksum256* full_hash) {
        internal_use_do_not_use::current_hash(simple_hash, full_hash);
    }
}
