//
// $Id$
//
// -------------------------------------------------------------------------
// This file is part of ZeroBugs, Copyright (c) 2010 Cristian L. Vlasceanu
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// -------------------------------------------------------------------------

#include <dwarf.h>

#include "private/log.h"
#include "error.h"
#include "impl.h"
#include "encoding_attr.h"

using namespace Dwarf;
using namespace std;


EncodingAttr::EncodingAttr(Dwarf_Debug dbg, Dwarf_Die die)
    : Attribute(dbg, die, DW_AT_encoding)
{
}


Dwarf_Unsigned EncodingAttr::value() const
{
    Dwarf_Error err = 0;
    Dwarf_Unsigned enc = 0;

    switch (form())
    {
    case DW_FORM_data1:
        if (dwarf_formudata(attr(), &enc, &err) == DW_DLV_ERROR)
        {
            THROW_ERROR(dbg(), err);
        }
        break;

    default:
        LOG_WARN << "EncodingAttr::value() form 0x" << hex
                 << form() << dec << " unhandled." << endl;
        break;
    }

    return enc;
}
// vim: tabstop=4:softtabstop=4:expandtab:shiftwidth=4

