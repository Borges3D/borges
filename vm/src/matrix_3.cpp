#include "matrix_3.h"
#include "deserializer.h"
#include "serializer.h"
#include <cassert>

namespace Borges {

double
Matrix_3::Const_row::operator[](std::size_t j) const
{
    assert(j < 3);
    return mi_[j];
}

Matrix_3::Const_row::Const_row(const double* mi) : mi_(mi)
{
}

Matrix_3::Matrix_3(std::initializer_list<std::initializer_list<double>> m)
{
    assert(m.size() == 3);
    for (std::size_t i = 0; i < 3; ++i) {
        std::initializer_list<double> row = m.begin()[i];
        assert(row.size() == 3);
        for (std::size_t j = 0; j < 3; ++j) {
            m_[i][j] = row.begin()[j];
        }
    }
}

Matrix_3::Matrix_3(const double (*m)[3])
{
    for (std::size_t i = 0; i < 3; ++i) {
        const double* row = m[i];
        for (std::size_t j = 0; j < 3; ++j) {
            m_[i][j] = row[j];
        }
    }
}

auto
Matrix_3::operator[](std::size_t i) const -> Const_row
{
    assert(i < 3);
    return Const_row(m_[i]);
}

void
serialize(Serializer& s, const Matrix_3& m)
{
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            serialize(s, m[i][j]);
        }
    }
}

void
deserialize(Deserializer& d, Matrix_3* p)
{
    double m[3][3];
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
            m[i][j] = deserialize<double>(d);
        }
    }
    new (p) Matrix_3(m);
}

Matrix_3
identity_matrix_3()
{
    return Matrix_3({{1, 0, 0}, {0, 1, 0}, {0, 0, 1}});
}

Matrix_3
operator*(const Matrix_3& m1, const Matrix_3& m2)
{
    double m[3][3];
    for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
          m[i][j] = 0.0;
          for (std::size_t k = 0; k < 3; ++k) {
              m[i][j] += m1[i][k] * m2[k][j];
          }
        }
    }
    return Matrix_3(m);
}

} // namespace Borges
