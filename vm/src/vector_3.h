#ifndef BORGES_VECTOR_3_H
#define BORGES_VECTOR_3_H

#include "type.h"
#include <initializer_list>

namespace Borges {

class Serializer;
class Deserializer;
class Matrix_4;

class Vector_3 {
public:
    explicit Vector_3(std::initializer_list<double> v);
    explicit Vector_3(const double v[3]);
    double operator[](std::size_t i) const;

private:
    double v_[3];
};

template <>
struct Type_of<Vector_3>
    : std::integral_constant<Type, Type::vector_3> {
};

template <>
struct Type_of<std::vector<Vector_3>>
    : std::integral_constant<Type, Type::vector_3_array> {
};

void serialize(Serializer& serializer, const Vector_3& v);
void deserialize(Deserializer& deserializer, Vector_3* p);
Vector_3 operator*(const Matrix_4& m, const Vector_3& v);

} // namespace Borges

#endif // BORGES_VECTOR_3_H
