#ifndef BORGES_TYPE_H
#define BORGES_TYPE_H

#include <cstddef>
#include <type_traits>
#include <vector>

namespace Borges {

class Serializer;
class Deserializer;

// Identifies the run-time type of a value.
enum class Type : std::size_t {
    boolean,
    number,
    vector_2,
    vector_3,
    matrix_3,
    matrix_4,
    boolean_array,
    number_array,
    vector_2_array,
    vector_3_array,
    matrix_3_array,
    matrix_4_array
};

// Obtains the run-time type of a value of type `T`.
template <typename T>
struct Type_of;

template <>
struct Type_of<bool> : std::integral_constant<Type, Type::boolean> {
};

template <>
struct Type_of<double> : std::integral_constant<Type, Type::number> {
};

template <>
struct Type_of<std::vector<bool>>
    : std::integral_constant<Type, Type::boolean_array> {
};

template <>
struct Type_of<std::vector<double>>
    : std::integral_constant<Type, Type::number_array> {
};

void serialize(Serializer& s, Type t);
void deserialize(Deserializer& d, Type* p);

} // namespace Borges

#endif // BORGES_TYPE_H
