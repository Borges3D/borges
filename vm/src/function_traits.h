#ifndef BORGES_FUNCTION_TRAITS_H
#define BORGES_FUNCTION_TRAITS_H

#include <cstddef>

namespace Borges {

template <typename... Ts>
struct Parameters {
    static constexpr std::size_t size = sizeof...(Ts);
};

// Obtains the properties of a function of type `F`.
template <typename F>
struct Function_traits : Function_traits<decltype(F::operator())> {
};

template <typename R, typename... Ts>
struct Function_traits<R(Ts...)> {
    using Result = R;
    using Parameters = Parameters<Ts...>;
};

template <typename R, typename... Ts>
struct Function_traits<R (*)(Ts...)> : Function_traits<R(Ts...)> {
};

} // namespace Borges

#endif // BORGES_FUNCTION_TRAITS_H
