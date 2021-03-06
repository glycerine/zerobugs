// -------------------------------------------------------------------------
// This file is part of ZeroBugs, Copyright (c) 2010 Cristian L. Vlasceanu
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// -------------------------------------------------------------------------
//
// $Id$
//
#if DEBUG
 #include <iostream>
#endif
#include "public/function.h"
#include "public/variable.h"
#include "private/generic_attr.h"
#include "inlined_instance.h"
#include "utils.h"

using namespace std;
using namespace Dwarf;


InlinedInstance::InlinedInstance(Dwarf_Debug dbg, Dwarf_Die die)
    : Block(dbg, die)
{
}


std::shared_ptr<Dwarf::Function> InlinedInstance::function() const
{
    // use DW_AT_abstract origin to get the inlined function die
    std::shared_ptr<Dwarf::Function> fun =
        std::dynamic_pointer_cast<Function>(check_indirect(false));

    if (fun)
    {
        if ((fun->low_pc() == 0) && (fun->high_pc() == 0))
        {
            fun->set_range(low_pc(), high_pc());
        }
    }
    return fun;
}


List<Parameter> InlinedInstance::params() const
{
#ifdef DEBUG
    if (std::shared_ptr<Function> fun = function())
    {
        clog << __func__ << ": " << fun->name() << endl;
    }
#endif
    return List<ParameterT<InlinedInstance> >(dbg(), die());
}


List<VariableT<InlinedInstance> > InlinedInstance::variables() const
{
#ifdef DEBUG
    if (std::shared_ptr<Function> fun = function())
    {
        clog << __func__ << ": " << fun->name() << endl;
    }
#endif
    return List<VariableT<InlinedInstance> >(dbg(), die());
}


size_t InlinedInstance::call_file() const
{
    size_t file = 0;
    if (Utils::has_attr(dbg(), die(), DW_AT_call_file))
    {
        GenericAttr<DW_AT_call_file, size_t> attr(dbg(), die());
        file = attr.value();
    }
    return file;
}


size_t InlinedInstance::call_line() const
{
    size_t line = 0;
    if (Utils::has_attr(dbg(), die(), DW_AT_call_line))
    {
        GenericAttr<DW_AT_call_line, size_t> attr(dbg(), die());
        line = attr.value();
    }
    return line;
}
// vim: tabstop=4:softtabstop=4:expandtab:shiftwidth=4
