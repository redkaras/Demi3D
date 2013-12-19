
/********************************************************************
    File:       SmartPtr.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include <memory>

#define TR1_PREFIX std

namespace Demi
{
    using TR1_PREFIX::shared_ptr;
    using TR1_PREFIX::weak_ptr;
    using TR1_PREFIX::enable_shared_from_this;
    using TR1_PREFIX::make_shared;
}