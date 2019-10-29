#pragma once

#include "action.hpp"
#include "name.hpp"

#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/mp11/tuple.hpp>

namespace ftl {

    /**
     * @defgroup dispatcher Dispatcher
     * @ingroup contracts
     * @brief Defines C++ functions to dispatch action to proper action handler inside a contract
     */

    /**
     * Unpack the received action and execute the correponding action handler
     *
     * @ingroup dispatcher
     * @tparam T - The contract class that has the correponding action handler, this contract should be derived from ftl::contract
     * @tparam RT - The type that the action handler returns
     * @tparam Args - The arguments that the action handler accepts, i.e. members of the action
     * @param func - The action handler
     * @return true
     */
    template<typename T, typename RT, typename... Args>
    bool execute_action(RT (T::*func)(Args...)) {
        size_t size = action_data_size();

        //using malloc/free here potentially is not exception-safe, although WASM doesn't support exceptions
        constexpr size_t max_stack_buffer_size = 512;
        void *buffer = nullptr;
        if (size > 0) {
            buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
            read_action_data(buffer, size);
        }

        std::tuple < std::decay_t < Args >...> args;
        datastream<const char *> ds((char *) buffer, size);
        ds >> args;

        T inst;
        auto f2 = [&](auto... a) {
            return ((&inst)->*func)(a...);
        };
        RT rt = std::apply(f2, args);
        size_t result_size = pack_size(rt);
        if (result_size > 0) {
            void *result_buffer = max_stack_buffer_size < result_size ? malloc(result_size) : alloca(result_size);
            ftl::datastream<char *> result_ds((char *) result_buffer, result_size);
            result_ds << rt;
            internal_use_do_not_use::set_result(result_buffer, result_size);
        }

        if (max_stack_buffer_size < size) {
            free(buffer);
        }
        return true;
    }

    template<typename T, typename... Args>
    bool execute_action(void (T::*func)(Args...)) {
        size_t size = action_data_size();

        //using malloc/free here potentially is not exception-safe, although WASM doesn't support exceptions
        constexpr size_t max_stack_buffer_size = 512;
        void *buffer = nullptr;
        if (size > 0) {
            buffer = max_stack_buffer_size < size ? malloc(size) : alloca(size);
            read_action_data(buffer, size);
        }

        std::tuple < std::decay_t < Args >...> args;
        datastream<const char *> ds((char *) buffer, size);
        ds >> args;

        T inst;
        auto f2 = [&](auto... a) {
            ((&inst)->*func)(a...);
        };
        std::apply(f2, args);

        if (max_stack_buffer_size < size) {
            free(buffer);
        }
        return true;
    }

    /// @cond INTERNAL

    // Helper macro for FTL_DISPATCH_INTERNAL
#define FTL_DISPATCH_INTERNAL(r, OP, elem) \
    case ftl::name( BOOST_PP_STRINGIZE(elem) ).value: \
       ftl::execute_action( &OP::elem ); \
       break;

    // Helper macro for FTL_DISPATCH
#define FTL_DISPATCH_HELPER(TYPE, MEMBERS) \
    BOOST_PP_SEQ_FOR_EACH( FTL_DISPATCH_INTERNAL, TYPE, MEMBERS )

/// @endcond

/**
 * Convenient macro to create contract apply handler
 *
 * @ingroup dispatcher
 * @note To be able to use this macro, the contract needs to be derived from ftl::contract
 * @param TYPE - The class name of the contract
 * @param MEMBERS - The sequence of available actions supported by this contract
 * @endcode
 */
#define FTL_DISPATCH(TYPE, MEMBERS) \
extern "C" { \
   [[ftl::wasm_entry]] \
   void apply( uint64_t action ) { \
         switch( action ) { \
            FTL_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: ftl_exit(0); */ \
   } \
} \

}
