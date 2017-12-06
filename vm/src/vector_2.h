#ifndef BORGES_VECTOR_2_H
#define BORGES_VECTOR_2_H

#include "type.h"
#include <initializer_list>

namespace Borges {

class Serializer;
class Deserializer;
class Matrix_3;

class Vector_2 {
public:
    explicit Vector_2(std::initializer_list<double> v);
    explicit Vector_2(const double v[2]);
    double operator[](std::size_t i) const;

private:
    double v_[2];
};

template <>
struct Type_of<Vector_2>
    : std::integral_constant<Type, Type::vector_2> {
};

template <>
struct Type_of<std::vector<Vector_2>>
    : std::integral_constant<Type, Type::vector_2_array> {
};

void serialize(Serializer& serializer, const Vector_2& v);
void deserialize(Deserializer& deserializer, Vector_2* p);
Vector_2 operator*(const Matrix_3& m, const Vector_2& v);

} // namespace Borges

#endif // BORGES_VECTOR_2_H
