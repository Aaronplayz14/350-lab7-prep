#ifndef PACK_H
#define PACK_H

#include <type_traits>

namespace pack_detail {

template <typename... Args>
constexpr int packSize = static_cast<int>(sizeof...(Args));

template <int QueryIndex, typename... Args>
struct _packIndexToType {
    static_assert(QueryIndex >= 0, "QueryIndex cannot be negative");
    static_assert(QueryIndex < static_cast<int>(sizeof...(Args)), "QueryIndex is past the end of the pack");
};

template <typename First, typename... Rest>
struct _packIndexToType<0, First, Rest...> {
    using type = First;
};

template <int QueryIndex, typename First, typename... Rest>
    requires(QueryIndex > 0)
struct _packIndexToType<QueryIndex, First, Rest...>
    : _packIndexToType<QueryIndex - 1, Rest...> {};

template <int QueryIndex, typename... Args>
using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;

template <int Accumulated, typename QueryType, typename... Args>
constexpr int _packTypeToIndex = -1;

template <int Accumulated, typename QueryType, typename First, typename... Rest>
constexpr int _packTypeToIndex<Accumulated, QueryType, First, Rest...> =
    std::is_same_v<QueryType, First>
        ? Accumulated
        : _packTypeToIndex<Accumulated + 1, QueryType, Rest...>;

template <typename QueryType, typename... Args>
constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;

} 

template <typename... Ts>
struct Pack {
    static constexpr int size = pack_detail::packSize<Ts...>;

    template <int QueryIndex>
    using type = pack_detail::packIndexToType<QueryIndex, Ts...>;

    template <typename QueryType>
    static constexpr int index = pack_detail::packTypeToIndex<QueryType, Ts...>;
};

#endif // PACK_H
