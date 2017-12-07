#include "vector_2.h"
#include "deserializer.h"
#include "matrix_3.h"
#include "serializer.h"
#include <cassert>

namespace Borges {

Vector_2::Vector_2(std::initializer_list<double> v)
{
    assert(v.size() == 2);
    for (std::size_t i = 0; i < 2; ++i) {
        v_[i] = v.begin()[i];
    }
}

Vector_2::Vector_2(const double v[2])
{
    for (std::size_t i = 0; i < 2; ++i) {
        v_[i] = v[i];
    }
}

double
Vector_2::operator[](std::size_t i) const
{
    assert(i < 2);
    return v_[i];
}

void
serialize(Serializer& s, const Vector_2& v)
{
    for (std::size_t i = 0; i < 2; ++i) {
        serialize(s, v[i]);
    }
}

void
deserialize(Deserializer& d, Vector_2* p)
{
    double v[2];
    for (std::size_t i = 0; i < 2; ++i) {
        v[i] = deserialize<double>(d);
    }
    new (p) Vector_2(v);
}

Vector_2
operator*(const Matrix_3& m, const Vector_2& v)
{
  double mv[2];
  for (std::size_t i = 0; i < 2; ++i) {
    mv[i] = 0.0;
    for (std::size_t j = 0; j < 2; ++j) {
      mv[i] += m[i][j] * v[j];
    }
    mv[i] += m[i][2];
  }
  return Vector_2(mv);
}

} // namespace Borges
