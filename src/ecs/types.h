#ifndef EKSAMENECS17_TYPES_H
#define EKSAMENECS17_TYPES_H

#include <cstdint>
#include <algorithm>

using Index = uint32_t;
using Offset = uint32_t;
using ComponentMask = uint32_t;

template <typename T, typename... Ts>
struct ComponentIndex;
template <typename T, typename... Ts>
struct ComponentIndex<T, T, Ts...> : std::integral_constant<size_t, 0> {};
template <typename T, typename U, typename... Ts>
struct ComponentIndex<T, U, Ts...> : std::integral_constant<size_t, 1 + ComponentIndex<T, Ts...>::value> {};

#endif
