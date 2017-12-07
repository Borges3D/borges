#include "vector_3.h"
#include "deserializer.h"
#include "matrix_4.h"
#include "serializer.h"
#include <cassert>

namespace Borges {

Vector_3::Vector_3(std::initializer_list<double> v)
{
    assert(v.size() == 3);
    for (std::size_t i = 0; i < 3; ++i) {
        v_[i] = v.begin()[i];
    }
}

Vector_3::Vector_3(const double v[3])
{
    for (std::size_t i = 0; i < 3; ++i) {
        v_[i] = v[i];
    }
}

double
Vector_3::operator[](std::size_t i) const
{
    assert(i < 3);
    return v_[i];
}

void
serialize(Serializer& s, const Vector_3& v)
{
    for (std::size_t i = 0; i < 3; ++i) {
        serialize(s, v[i]);
    }
}

void
deserialize(Deserializer& d, Vector_3* p)
{
    double v[3];
    for (std::size_t i = 0; i < 3; ++i) {
        v[i] = deserialize<double>(d);
    }
    new (p) Vector_3(v);
}

Vector_3
operator*(const Matrix_4& m, const Vector_3& v)
{
  double mv[3];
  for (std::size_t i = 0; i < 3; ++i) {
    mv[i] = 0.0;
    for (std::size_t j = 0; j < 3; ++j) {
      mv[i] += m[i][j] * v[j];
    }
    mv[i] += m[i][3];
  }
  return Vector_3(mv);
}

} // namespace Borges
