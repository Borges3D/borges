#include "value.h"
#include "deserializer.h"
#include "matrix_3.h"
#include "matrix_4.h"
#include "serializer.h"
#include "vector_2.h"
#include "vector_3.h"
#include <cassert>
#include <utility>

namespace Borges {

Value::Value(bool b): type_(Type::boolean), boolean_(b)
{
}

Value::Value(double x) : type_(Type::number), number_(x)
{
}

Value::Value(const Vector_2& v)
    : type_(Type::vector_2),
      vector_2_(std::unique_ptr<const Vector_2>(new Vector_2(v)))
{
}

Value::Value(const Vector_3& v)
    : type_(Type::vector_3),
      vector_3_(std::unique_ptr<const Vector_3>(new Vector_3(v)))
{
}

Value::Value(const Matrix_3& m)
    : type_(Type::matrix_3),
      matrix_3_(std::unique_ptr<const Matrix_3>(new Matrix_3(m)))
{
}

Value::Value(const Matrix_4& m)
    : type_(Type::matrix_4),
      matrix_4_(std::unique_ptr<const Matrix_4>(new Matrix_4(m)))
{
}

Value::Value(std::vector<bool> bs)
    : type_(Type::boolean_array),
      boolean_array_(std::shared_ptr<const std::vector<bool>>(
          new std::vector<bool>(std::move(bs))))
{
}

Value::Value(std::vector<double> xs)
    : type_(Type::number_array),
      number_array_(std::shared_ptr<const std::vector<double>>(
          new std::vector<double>(std::move(xs))))
{
}

Value::Value(std::vector<Vector_2> vs)
    : type_(Type::vector_2_array),
      vector_2_array_(std::shared_ptr<const std::vector<Vector_2>>(
          new std::vector<Vector_2>(std::move(vs))))
{
}

Value::Value(std::vector<Vector_3> vs)
    : type_(Type::vector_3_array),
      vector_3_array_(std::shared_ptr<const std::vector<Vector_3>>(
          new std::vector<Vector_3>(std::move(vs))))
{
}

Value::Value(std::vector<Matrix_3> ms)
    : type_(Type::matrix_3_array),
      matrix_3_array_(std::shared_ptr<const std::vector<Matrix_3>>(
          new std::vector<Matrix_3>(std::move(ms))))
{
}

Value::Value(std::vector<Matrix_4> ms)
    : type_(Type::matrix_4_array),
      matrix_4_array_(std::shared_ptr<const std::vector<Matrix_4>>(
          new std::vector<Matrix_4>(std::move(ms))))
{
}

Value::Value(const Value& x)
{
    construct(x);
}

Value::Value(Value&& x)
{
    construct(x);
}

Value&
Value::operator=(const Value& x)
{
    if (this != &x) {
        destroy();
        construct(x);
    }
    return *this;
}

Value&
Value::operator=(Value&& x)
{
    if (this != &x) {
        destroy();
        construct(x);
    }
    return *this;
}

Value::~Value()
{
    destroy();
}

Type
Value::type() const
{
    return type_;
}

const bool&
Value::boolean() const
{
    assert(type_ == Type::boolean);
    return boolean_;
}

const double&
Value::number() const
{
    assert(type_ == Type::number);
    return number_;
}

const Vector_2&
Value::vector_2() const
{
    assert(type_ == Type::vector_2);
    return *vector_2_;
}

const Vector_3&
Value::vector_3() const
{
    assert(type_ == Type::vector_3);
    return *vector_3_;
}

const Matrix_3&
Value::matrix_3() const
{
    assert(type_ == Type::matrix_3);
    return *matrix_3_;
}

const Matrix_4&
Value::matrix_4() const
{
    assert(type_ == Type::matrix_4);
    return *matrix_4_;
}

const std::vector<bool>&
Value::boolean_array() const
{
    assert(type_ == Type::boolean_array);
    return *boolean_array_;
}

const std::vector<double>&
Value::number_array() const
{
    assert(type_ == Type::number_array);
    return *number_array_;
}

const std::vector<Vector_2>&
Value::vector_2_array() const
{
    assert(type_ == Type::vector_2_array);
    return *vector_2_array_;
}

const std::vector<Vector_3>&
Value::vector_3_array() const
{
    assert(type_ == Type::vector_3_array);
    return *vector_3_array_;
}

const std::vector<Matrix_3>&
Value::matrix_3_array() const
{
    assert(type_ == Type::matrix_3_array);
    return *matrix_3_array_;
}

const std::vector<Matrix_4>&
Value::matrix_4_array() const
{
    assert(type_ == Type::matrix_4_array);
    return *matrix_4_array_;
}

void
Value::construct(const Value& x)
{
    type_ = x.type_;
    switch (type_) {
    case Type::boolean:
        boolean_ = x.boolean_;
        break;
    case Type::number:
        number_ = x.number_;
        break;
    case Type::vector_2:
        new (&vector_2_)
            std::unique_ptr<const Vector_2>(new Vector_2(*x.vector_2_));
        break;
    case Type::vector_3:
        new (&vector_3_)
            std::unique_ptr<const Vector_3>(new Vector_3(*x.vector_3_));
        break;
    case Type::matrix_3:
        new (&matrix_3_)
            std::unique_ptr<const Matrix_3>(new Matrix_3(*x.matrix_3_));
        break;
    case Type::matrix_4:
        new (&matrix_4_)
            std::unique_ptr<const Matrix_4>(new Matrix_4(*x.matrix_4_));
        break;
    case Type::boolean_array:
        new (&boolean_array_)
            std::shared_ptr<const std::vector<bool>>(x.boolean_array_);
        break;
    case Type::number_array:
        new (&number_array_)
            std::shared_ptr<const std::vector<double>>(x.number_array_);
        break;
    case Type::vector_2_array:
        new (&vector_2_array_)
            std::shared_ptr<const std::vector<Vector_2>>(x.vector_2_array_);
        break;
    case Type::vector_3_array:
        new (&vector_2_array_)
            std::shared_ptr<const std::vector<Vector_3>>(x.vector_3_array_);
        break;
    case Type::matrix_3_array:
        new (&matrix_3_array_)
            std::shared_ptr<const std::vector<Matrix_3>>(x.matrix_3_array_);
        break;
    case Type::matrix_4_array:
        new (&matrix_4_array_)
            std::shared_ptr<const std::vector<Matrix_4>>(x.matrix_4_array_);
        break;
    }
}

void
Value::construct(Value&& x)
{
    type_ = x.type_;
    switch (type_) {
    case Type::boolean:
        boolean_ = x.boolean_;
        break;
    case Type::number:
        number_ = x.number_;
        break;
    case Type::vector_2:
        new (&vector_2_)
            std::unique_ptr<const Vector_2>(new Vector_2(*x.vector_2_));
        break;
    case Type::vector_3:
        new (&vector_3_)
            std::unique_ptr<const Vector_3>(new Vector_3(*x.vector_3_));
        break;
    case Type::matrix_3:
        new (&matrix_3_)
            std::unique_ptr<const Matrix_3>(new Matrix_3(*x.matrix_3_));
        break;
    case Type::matrix_4:
        new (&matrix_4_)
            std::unique_ptr<const Matrix_4>(new Matrix_4(*x.matrix_4_));
        break;
    case Type::boolean_array:
        new (&boolean_array_) std::shared_ptr<const std::vector<bool>>(
            std::move(x.boolean_array_));
        break;
    case Type::number_array:
        new (&number_array_) std::shared_ptr<const std::vector<double>>(
            std::move(x.number_array_));
        break;
    case Type::vector_2_array:
        new (&vector_2_array_) std::shared_ptr<const std::vector<Vector_2>>(
            std::move(x.vector_2_array_));
        break;
    case Type::vector_3_array:
        new (&vector_2_array_) std::shared_ptr<const std::vector<Vector_3>>(
            std::move(x.vector_3_array_));
        break;
    case Type::matrix_3_array:
        new (&matrix_3_array_) std::shared_ptr<const std::vector<Matrix_3>>(
            std::move(x.matrix_3_array_));
        break;
    case Type::matrix_4_array:
        new (&matrix_4_array_) std::shared_ptr<const std::vector<Matrix_4>>(
            std::move(x.matrix_4_array_));
        break;
    }
}

void
Value::destroy()
{
    switch (type_) {
    case Type::boolean:
        break;
    case Type::number:
        break;
    case Type::vector_2:
        vector_2_.~unique_ptr<const Vector_2>();
        break;
    case Type::vector_3:
        vector_3_.~unique_ptr<const Vector_3>();
        break;
    case Type::matrix_3:
        matrix_3_.~unique_ptr<const Matrix_3>();
        break;
    case Type::matrix_4:
        matrix_4_.~unique_ptr<const Matrix_4>();
        break;
    case Type::boolean_array:
        boolean_array_.~shared_ptr<const std::vector<bool>>();
        break;
    case Type::number_array:
        number_array_.~shared_ptr<const std::vector<double>>();
        break;
    case Type::vector_2_array:
        vector_2_array_.~shared_ptr<const std::vector<Vector_2>>();
        break;
    case Type::vector_3_array:
        vector_3_array_.~shared_ptr<const std::vector<Vector_3>>();
        break;
    case Type::matrix_3_array:
        matrix_3_array_.~shared_ptr<const std::vector<Matrix_3>>();
        break;
    case Type::matrix_4_array:
        matrix_4_array_.~shared_ptr<const std::vector<Matrix_4>>();
        break;
    }
}

void
serialize(Serializer& s, const Value& x)
{
    Type t = x.type();
    serialize(s, t);
    switch (t) {
    case Type::boolean:
        serialize(s, x.boolean());
        break;
    case Type::number:
        serialize(s, x.number());
        break;
    case Type::vector_2:
        serialize(s, x.vector_2());
        break;
    case Type::vector_3:
        serialize(s, x.vector_3());
        break;
    case Type::matrix_3:
        serialize(s, x.matrix_3());
        break;
    case Type::matrix_4:
        serialize(s, x.matrix_4());
        break;
    case Type::boolean_array:
        serialize(s, x.boolean_array());
        break;
    case Type::number_array:
        serialize(s, x.number_array());
        break;
    case Type::vector_2_array:
        serialize(s, x.vector_2_array());
        break;
    case Type::vector_3_array:
        serialize(s, x.vector_3_array());
        break;
    case Type::matrix_3_array:
        serialize(s, x.matrix_3_array());
        break;
    case Type::matrix_4_array:
        serialize(s, x.matrix_4_array());
        break;
    }
}

void
deserialize(Deserializer& d, Value* p)
{
    switch (deserialize<Type>(d)) {
    case Type::boolean:
        new (p) Value(deserialize<bool>(d));
        break;
    case Type::number:
        new (p) Value(deserialize<double>(d));
        break;
    case Type::vector_2:
        new (p) Value(deserialize<Vector_2>(d));
        break;
    case Type::vector_3:
        new (p) Value(deserialize<Vector_3>(d));
        break;
    case Type::matrix_3:
        new (p) Value(deserialize<Matrix_3>(d));
        break;
    case Type::matrix_4:
        new (p) Value(deserialize<Matrix_4>(d));
        break;
    case Type::boolean_array:
        new (p) Value(deserialize<std::vector<bool>>(d));
        break;
    case Type::number_array:
        new (p) Value(deserialize<std::vector<double>>(d));
        break;
    case Type::vector_2_array:
        new (p) Value(deserialize<std::vector<Vector_2>>(d));
        break;
    case Type::vector_3_array:
        new (p) Value(deserialize<std::vector<Vector_3>>(d));
        break;
    case Type::matrix_3_array:
        new (p) Value(deserialize<std::vector<Matrix_3>>(d));
        break;
    case Type::matrix_4_array:
        new (p) Value(deserialize<std::vector<Matrix_4>>(d));
        break;
    }
}

template <>
const bool&
get<bool>(const Value& x)
{
    return x.boolean();
}

template <>
const double&
get<double>(const Value& x)
{
    return x.number();
}

template <>
const Vector_2&
get<Vector_2>(const Value& x)
{
    return x.vector_2();
}

template <>
const Vector_3&
get<Vector_3>(const Value& x)
{
    return x.vector_3();
}

template <>
const Matrix_3&
get<Matrix_3>(const Value& x)
{
    return x.matrix_3();
}

template <>
const Matrix_4&
get<Matrix_4>(const Value& x)
{
    return x.matrix_4();
}

template <>
const std::vector<bool>&
get<std::vector<bool>>(const Value& x)
{
    return x.boolean_array();
}

template <>
const std::vector<double>&
get<std::vector<double>>(const Value& x)
{
    return x.number_array();
}

template <>
const std::vector<Vector_2>&
get<std::vector<Vector_2>>(const Value& x)
{
    return x.vector_2_array();
}

template <>
const std::vector<Vector_3>&
get<std::vector<Vector_3>>(const Value& x)
{
    return x.vector_3_array();
}

template <>
const std::vector<Matrix_3>&
get<std::vector<Matrix_3>>(const Value& x)
{
    return x.matrix_3_array();
}

template <>
const std::vector<Matrix_4>&
get<std::vector<Matrix_4>>(const Value& x)
{
    return x.matrix_4_array();
}

} // namespace Borges
