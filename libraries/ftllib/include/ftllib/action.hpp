#pragma once

#include <cstdlib>

#include "base.hpp"
#include "datastream.hpp"
#include "address.hpp"
#include "name.hpp"

namespace ftl {
    /**
     *  @defgroup action Action
     *  @ingroup contracts
     *  @brief Defines type-safe C++ wrappers for querying action and sending action
     *  @note There are some methods from the @ref action that can be used directly from C++
     */

    /**
     *  Copy up to length bytes of current action data to the specified location
     *
     *  @ingroup action
     *  @param msg - a pointer where up to length bytes of the current action data will be copied
     *  @param len - len of the current action data to be copied, 0 to report required size
     *  @return the number of bytes copied to msg, or number of bytes that can be copied if len==0 passed
     *  @pre `msg` is a valid pointer to a range of memory at least `len` bytes long
     *  @post `msg` is filled with packed action data
     */
    inline uint32_t read_action_data(void *msg, uint32_t len) {
        return internal_use_do_not_use::read_action_data(msg, len);
    }

    /**
     * Get the length of the current action's data field. This method is useful for dynamically sized actions
     *
     * @return the length of the current action's data field
     */
    inline uint32_t action_data_size() {
        return internal_use_do_not_use::action_data_size();
    }

    /**
     * @brief Contract transter amount tokens to addr.
     * @param addr - the address send tokens to.
     * @param amount - the amount of tokens to be transferred
     */
    inline void transfer(address addr, uint64_t amount) {
        internal_use_do_not_use::transfer(addr.addr, ADDR_LEN, amount);
    }

    /**
      * @brief Get the amount of the token the transaction transfer.
      * @reuturn the amount of the token the transaction transfer.
      * @retval <uint64_t>
      */
    inline uint64_t get_transfer_amount() {
        return internal_use_do_not_use::get_amount();
    }

    template<typename>
    struct action;

    template<typename T, typename ...Args>
    struct action<T (*)(Args...)> {
        static int call(T *result, address contract, name act, uint64_t amount, int storage_delegate, int user_delegate, Args... args) {
            std::tuple<Args...> t = std::tuple<Args...>(args...);
            size_t size = pack_size(act) + pack_size(t);
            void *buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
            ftl::datastream<char *> ds((char *) buffer, size);
            ds << act;
            ds << t;

            int ret = internal_use_do_not_use::call_action(contract.addr, ADDR_LEN, buffer, size, amount, storage_delegate, user_delegate);
            size_t result_size = internal_use_do_not_use::call_result(NULL, 0);
            if (result_size > 0) {
                void *result_buffer = max_stack_buffer_size < result_size ? malloc(result_size) : alloca(result_size);
                internal_use_do_not_use::call_result(result_buffer, result_size);
                ftl::datastream<char *> result_ds((char *) result_buffer, result_size);
                result_ds >> *result;

                if (max_stack_buffer_size < result_size) {
                    free(result_buffer);
                }
            }

            if(max_stack_buffer_size < size) {
                free(buffer);
            }
            return ret;
        }
    };

    template<typename ...Args>
    struct action<void (*)(Args...)> {
        static int call(void *result, address contract, name act, uint64_t amount, int storage_delegate, int user_delegate, Args... args) {
            std::tuple<Args...> t = std::tuple<Args...>(args...);
            size_t size = pack_size(act) + pack_size(t);
            void *buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
            ftl::datastream<char *> ds((char *) buffer, size);
            ds << act;
            ds << t;

            int ret = internal_use_do_not_use::call_action(contract.addr, ADDR_LEN, buffer, size, amount, storage_delegate, user_delegate);

            if(max_stack_buffer_size < size) {
                free(buffer);
            }
            return ret;
        }
    };

    #define CALL(presult, contract, act, func, amount, ...) ftl::action<decltype(&func)>::call(presult, contract, ftl::name(act), amount, 0, 0, __VA_ARGS__)
    #define DELEGATE_CALL(presult, contract, act, func, amount, ...) ftl::action<decltype(&func)>::call(presult, contract, ftl::name(act), amount, 1, 0, __VA_ARGS__)
    #define CONTRACT_CALL(presult, contract, act, func, amount, ...) ftl::action<decltype(&func)>::call(presult, contract, ftl::name(act), amount, 0, 1, __VA_ARGS__)
    #define CALL_NOARGS(presult, contract, act, func, amount) ftl::action<decltype(&func)>::call(presult, contract, ftl::name(act), amount, 0, 0)
    #define DELEGATE_CALL_NOARGS(presult, contract, act, func, amount) ftl::action<decltype(&func)>::call(presult, contract, ftl::name(act), amount, 1, 0)
    #define CONTRACT_CALL_NOARGS(presult, contract, act, func, amount) ftl::action<decltype(&func)>::call(presult, contract, ftl::name(act), amount, 0, 1)
}
