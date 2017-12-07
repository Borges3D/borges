#include "matrix_4.h"
#include "deserializer.h"
#include "serializer.h"
#include <cassert>

namespace Borges {

double
Matrix_4::Const_row::operator[](std::size_t j) const
{
    assert(j < 4);
    return mi_[j];
}

Matrix_4::Const_row::Const_row(const double* mi) : mi_(mi)
{
}

Matrix_4::Matrix_4(std::initializer_list<std::initializer_list<double>> m)
{
    assert(m.size() == 4);
    for (std::size_t i = 0; i < 4; ++i) {
        std::initializer_list<double> row = m.begin()[i];
        assert(row.size() == 4);
        for (std::size_t j = 0; j < 4; ++j) {
            m_[i][j] = row.begin()[j];
        }
    }
}

Matrix_4::Matrix_4(const double (*m)[4])
{
    for (std::size_t i = 0; i < 4; ++i) {
        const double* row = m[i];
        for (std::size_t j = 0; j < 4; ++j) {
            m_[i][j] = row[j];
        }
    }
}

auto
Matrix_4::operator[](std::size_t i) const -> Const_row
{
    assert(i < 4);
    return Const_row(m_[i]);
}

void
serialize(Serializer& s, const Matrix_4& m)
{
    for (std::size_t i = 0; i < 4; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            serialize(s, m[i][j]);
        }
    }
}

void
deserialize(Deserializer& d, Matrix_4* p)
{
    double m[4][4];
    for (std::size_t i = 0; i < 4; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
            m[i][j] = deserialize<double>(d);
        }
    }
    new (p) Matrix_4(m);
}

Matrix_4
identity_matrix_4()
{
    return Matrix_4({{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}});
}

Matrix_4
operator*(const Matrix_4& m1, const Matrix_4& m2)
{
    double m[4][4];
    for (std::size_t i = 0; i < 4; ++i) {
        for (std::size_t j = 0; j < 4; ++j) {
          m[i][j] = 0.0;
          for (std::size_t k = 0; k < 4; ++k) {
              m[i][j] += m1[i][k] * m2[k][j];
          }
        }
    }
    return Matrix_4(m);
}

} // namespace Borges
