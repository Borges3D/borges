#ifndef BORGES_FUNCTION_DESCRIPTOR_H
#define BORGES_FUNCTION_DESCRIPTOR_H

#include "function_traits.h"
#include "type.h"

namespace Borges {

// Describes the run-time type of a function. In particular, holds the run-time
// type of its result, the number of its parameters, and the run-time type of
// each of its parameters.
struct Function_descriptor {
    Type result;
    std::size_t parameters_size;
    const Type* parameters;
};

template <typename Parameters>
struct Describe_parameters;

template <typename... Ts>
struct Describe_parameters<Parameters<Ts...>> {
    static constexpr Type value[] = {Type_of<typename std::remove_const<
        typename std::remove_reference<Ts>::type>::type>::value...};
};

template <typename... Ts>
constexpr Type Describe_parameters<Parameters<Ts...>>::value[];

// Obtains a descriptor for a function of type `F`.
template <typename F>
class Describe_function {
private:
    using Function_traits = Function_traits<F>;
    using Result = typename Function_traits::Result;
    using Parameters = typename Function_traits::Parameters;

public:
    static constexpr Function_descriptor value = {
        Type_of<Result>::value,
        Parameters::size,
        Describe_parameters<Parameters>::value
    };
};

} // namespace Borges

#endif // BORGES_FUNCTION_DESCRIPTOR_H
