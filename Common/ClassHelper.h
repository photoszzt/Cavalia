#pragma once
#ifndef __COMMON_CLASS_HELPER_H__
#define __COMMON_CLASS_HELPER_H__
#include <type_traits>

// Put this in the declarations for a class to be uncopyable.
#define DISALLOW_COPY(TypeName) \
    TypeName(const TypeName&) = delete

// Put this in the declarations for a class to be unassignable.
#define DISALLOW_ASSIGN(TypeName) \
    TypeName& operator=(const TypeName&) = delete

// Put this in the declarations for a class to be uncopyable and unassignable.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    DISALLOW_COPY(TypeName);               \
    DISALLOW_ASSIGN(TypeName)

#define CHECK_STD_LAYOUT(type_, val) static_assert(std::is_standard_layout_v<type_> == val, "")
#define CHECK_TRIVIAL(type_, val) static_assert(std::is_trivial_v<type_> == val, "")

#endif // __COMMON_CLASS_HELPER_H__
