#ifndef BORGES_MATRIX_4_H
#define BORGES_MATRIX_4_H

#include "type.h"
#include <initializer_list>

namespace Borges {

class Serializer;
class Deserializer;

class Matrix_4 {
public:
    class Const_row {
    public:
        double operator[](std::size_t j) const;

    private:
        friend class Matrix_4;

        Const_row(const double* mi);

        const double* mi_;
    };

    explicit Matrix_4(std::initializer_list<std::initializer_list<double>> m);
    explicit Matrix_4(const double (*m)[4]);
    Const_row operator[](std::size_t i) const;

private:
    double m_[4][4];
};

template <>
struct Type_of<Matrix_4>
    : std::integral_constant<Type, Type::matrix_4> {
};

template <>
struct Type_of<std::vector<Matrix_4>>
    : std::integral_constant<Type, Type::matrix_4_array> {
};

void serialize(Serializer& s, const Matrix_4& m);
void deserialize(Deserializer& d, Matrix_4* p);
Matrix_4 identity_matrix_4();
Matrix_4 operator*(const Matrix_4& m1, const Matrix_4& m2);

} // namespace Borges

#endif // BORGES_MATRIX_4_H
