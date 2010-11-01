#ifndef ASSERT_H__261A2F0A_B1BC_4C90_9875_15426B1A7217
#define ASSERT_H__261A2F0A_B1BC_4C90_9875_15426B1A7217
//
// $Id: assert.h 714 2010-10-17 10:03:52Z root $
//
// -------------------------------------------------------------------------
// This file is part of ZeroBugs, Copyright (c) 2010 Cristian L. Vlasceanu
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// -------------------------------------------------------------------------

#include <cassert>
#include <string>
#include <stdexcept>


// Place holder, so that we can change the behavior
// on failure, from calling abort() to throwing an exception.
#define NUTS_ASSERT assert

#if DEBUG
 #define NUTS_EXHAUSTED_FIXED_MEM() exhausted_fixed_mem(__func__)
#else
 #define NUTS_EXHAUSTED_FIXED_MEM() // as nothing
#endif

namespace nuts
{
    static void inline exhausted_fixed_mem(const std::string& func)
    {
        throw std::runtime_error(func + ": fixed memory space exhausted");
    }
}
#endif // ASSERT_H__261A2F0A_B1BC_4C90_9875_15426B1A7217
// vim: tabstop=4:softtabstop=4:expandtab:shiftwidth=4