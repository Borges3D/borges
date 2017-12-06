#ifndef BORGES_MATRIX_3_H
#define BORGES_MATRIX_3_H

#include "type.h"
#include <initializer_list>

namespace Borges {

class Serializer;
class Deserializer;

class Matrix_3 {
public:
    class Const_row {
    public:
        double operator[](std::size_t j) const;

    private:
        friend class Matrix_3;

        Const_row(const double* mi);

        const double* mi_;
    };

    explicit Matrix_3(std::initializer_list<std::initializer_list<double>> m);
    explicit Matrix_3(const double (*m)[3]);
    Const_row operator[](std::size_t i) const;

private:
    double m_[3][3];
};

template <>
struct Type_of<Matrix_3>
    : std::integral_constant<Type, Type::matrix_3> {
};

template <>
struct Type_of<std::vector<Matrix_3>>
    : std::integral_constant<Type, Type::matrix_3_array> {
};

void serialize(Serializer& s, const Matrix_3& m);
void deserialize(Deserializer& d, Matrix_3* p);
Matrix_3 identity_matrix_3();
Matrix_3 operator*(const Matrix_3& m1, const Matrix_3& m2);

} // namespace Borges

#endif // BORGES_MATRIX_3_H
