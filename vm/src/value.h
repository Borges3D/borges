#ifndef BORGES_VALUE_H
#define BORGES_VALUE_H

#include "type.h"
#include <memory>

namespace Borges {

class Vector_2;
class Vector_3;
class Matrix_3;
class Matrix_4;

// Represents a value that can take on several different types.
//
// Since values will be stored on the stack, they need to take up as little
// storage as possible. Otherwise, each value would take up as much storage as
// the largest possible type it can take on. At the same time, copying a value
// needs to be as cheap as possible. We therefore adopt the following strategy
// for representing values:
//
// Small values are represented directly. Small values take up no more storage
// than a double or std::shared_ptr. Examples of small values are bools and
// doubles.
//
// Large static values are represented with a unique_ptr. Large static values
// take up more storage than a double or std::shared_ptr, but contain no
// pointers to additional storage, so they are cheap to copy. Examples of
// large static values are Vector_2 and Matrix_3.
//
// Large dynamic values are represented with a shared_ptr. Large dynamic values
// take up more storage than a double or std::shared_ptr, and contain one or
// more pointers to additional storage, so they are expensive to copy. Examples
// of large dynamic values are std::vector.
class Value {
public:
    explicit Value(bool b);
    explicit Value(double x);
    explicit Value(const Vector_2& v);
    explicit Value(const Vector_3& v);
    explicit Value(const Matrix_3& m);
    explicit Value(const Matrix_4& m);
    explicit Value(std::vector<bool> bs);
    explicit Value(std::vector<double> xs);
    explicit Value(std::vector<Vector_2> vs);
    explicit Value(std::vector<Vector_3> vs);
    explicit Value(std::vector<Matrix_3> ms);
    explicit Value(std::vector<Matrix_4> ms);
    Value(const Value& x);
    Value(Value&& x);
    Value& operator=(const Value& x);
    Value& operator=(Value&& x);
    ~Value();
    Type type() const;
    const bool& boolean() const;
    const double& number() const;
    const Vector_2& vector_2() const;
    const Vector_3& vector_3() const;
    const Matrix_3& matrix_3() const;
    const Matrix_4& matrix_4() const;
    const std::vector<bool>& boolean_array() const;
    const std::vector<double>& number_array() const;
    const std::vector<Vector_2>& vector_2_array() const;
    const std::vector<Vector_3>& vector_3_array() const;
    const std::vector<Matrix_3>& matrix_3_array() const;
    const std::vector<Matrix_4>& matrix_4_array() const;

private:
    void construct(const Value& other);
    void construct(Value&& other);
    void destroy();

    Type type_;
    union {
        bool boolean_;
        double number_;
        std::unique_ptr<const Vector_2> vector_2_;
        std::unique_ptr<const Vector_3> vector_3_;
        std::unique_ptr<const Matrix_3> matrix_3_;
        std::unique_ptr<const Matrix_4> matrix_4_;
        std::shared_ptr<const std::vector<bool>> boolean_array_;
        std::shared_ptr<const std::vector<double>> number_array_;
        std::shared_ptr<const std::vector<Vector_2>> vector_2_array_; 
        std::shared_ptr<const std::vector<Vector_3>> vector_3_array_; 
        std::shared_ptr<const std::vector<Matrix_3>> matrix_3_array_; 
        std::shared_ptr<const std::vector<Matrix_4>> matrix_4_array_; 
    };
};

void serialize(Serializer& s, const Value& x);
void deserialize(Deserializer& d, Value* p);

// Casts the value `x` to a value of type `T`. If `x` does not have type `T`,
// an assertion will be triggered. This function is needed in generic contexts,
// when we want to obtain a value of type `T`.
template <typename T>
const T& get(const Value& x);
template <>
const bool& get<bool>(const Value& x);
template <>
const double& get<double>(const Value& x);
template <>
const Vector_2& get<Vector_2>(const Value& x);
template <>
const Vector_3& get<Vector_3>(const Value& x);
template <>
const Matrix_3& get<Matrix_3>(const Value& x);
template <>
const Matrix_4& get<Matrix_4>(const Value& x);
template <>
const std::vector<bool>& get<std::vector<bool>>(const Value& x);
template <>
const std::vector<double>& get<std::vector<double>>(const Value& x);
template <>
const std::vector<Vector_2>& get<std::vector<Vector_2>>(const Value& x);
template <>
const std::vector<Vector_3>& get<std::vector<Vector_3>>(const Value& x);
template <>
const std::vector<Matrix_3>& get<std::vector<Matrix_3>>(const Value& x);
template <>
const std::vector<Matrix_4>& get<std::vector<Matrix_4>>(const Value& x);

// Returns true if the value `x` has type `T`.
template <typename T>
bool
is(const Value& x)
{
    return x.type() == Type_of<T>::value;
}

} // namespace Borges

#endif // BORGES_VALUE_H
