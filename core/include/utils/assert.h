#pragma once

#include <cstdlib>
#include "utils/log.h"
#include <fmt/format.h>

namespace core
{

class AssertException final : public std::exception
{
    using std::exception::exception;
};

}

#ifdef GPR_ASSERT
#define gpr_assert(Expr, Msg) \
    if(!(Expr)) \
    { \
        core::LogError(fmt::format("Assert failed:\t{}\nSource:\t\t{}, line {}", \
            #Msg, __FILE__,__LINE__)); \
        throw core::AssertException(Msg); \
    }
# ifdef GPR_ABORT_WARN

#define gpr_warn(Expr, Msg) \
    if(!(Expr)) \
    { \
        core::LogWarning(fmt::format("Warning Assert failed:\t{}\nSource:\t\t{}, line {}", \
            #Msg, __FILE__,__LINE__)); \
        throw core::AssertException(Msg); \
    }
# else

#define gpr_warn(Expr, Msg) \
    if (!(Expr)) \
    { \
        core::LogWarning(fmt::format("Warning Assert failed:\t{}\nSource:\t\t{}, line {}", \
        #Msg, __FILE__, __LINE__)); \
    }
# endif
#else
#define gpr_assert(Expr, Msg) \
    void(0);
#define gpr_warn(Expr, Msg) \
    void(0);
#endif
