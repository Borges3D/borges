#ifndef BORGES_DESERIALIZER_H
#define BORGES_DESERIALIZER_H

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Borges {

// Encapsulates the state necessary to deserialize C++ values from a buffer.
class Deserializer {
public:
    // Creates a deserializer that reads from a buffer with size `n` and data
    // pointed to by the pointer `p`.
    //
    // Note that the deserializer does not own `p`.
    Deserializer(std::size_t n, const double* p);
    void deserialize(double* p);
    template <typename T>
    void deserialize(std::shared_ptr<T>* p);

private:
    std::size_t buffer_size_;
    const double* buffer_data_;
    std::size_t buffer_index_;
    std::unordered_map<std::size_t, std::shared_ptr<void>> index_to_ptr_map_;
};

void deserialize(Deserializer& d, bool* p);
void deserialize(Deserializer& d, double* p);
void deserialize(Deserializer& d, std::size_t* p);

// This is the main deserialization function.
//
// Since C++ does not allow partial specialization of template functions, this
// function should not be specialized directly. If you want to make your type
// deserializable, you should add an overload of the helper function used by
// this function to construct a value in uninitialized storage instead.
template <typename T>
T
deserialize(Deserializer& d)
{
    // To deserialize a value of type `T`, we need to create a temporary value
    // to which the value to be deserialized can be assigned. Since `T` might
    // not have a default constructor, we don't create this temporary value
    // directly. Instead, we create a buffer of uninitialized storage `b` that
    // is properly aligned for values of type `T`, and then call a helper
    // function that directly constructs a value of type `T` in `b`.
    alignas(T) char b[sizeof(T)];
    T* p = reinterpret_cast<T*>(b);
    deserialize(d, p);
    // Make sure that the value stored in `b` is properly destructed, by first
    // moving it to the temporary value `v`, and then explicitly calling the
    // destructor on the pointer `p` to `b`. The explicit destructor call is
    // necessary because `T` might not have a move constructor, in which case
    // the value will be copied instead.
    T v(std::move(*p));
    p->~T();
    return std::move(v);
}

template <typename T>
void
Deserializer::deserialize(std::shared_ptr<T>* p)
{
    // Deserialize the index `i` associated with the pointer.
    std::size_t i = Borges::deserialize<std::size_t>(*this);
    // If there is a pointer associated with `i`, we have encountered the
    // pointer before. In this case, we can reconstruct the pointer from `i`
    // alone, by copy constructing it with the pointer associated with `i`.
    auto it = index_to_ptr_map_.find(i);
    if (it != index_to_ptr_map_.end()) {
        new (p) std::shared_ptr<T>(it->second);
        return;
    }
    // Otherwise, this is the first time we have encountered the pointer.
    // Deserialize the value being pointed to by the pointer, use it to
    // reconstruct the pointer, and associate the pointer with `i`, so that
    // subsequent occurences of the pointer can be reconstructed from `i` alone.
    std::shared_ptr<T> ptr(std::make_shared<T>(deserialize<T>(*this)));
    new (p) std::shared_ptr<T>(std::move(ptr));
    index_to_ptr_map_.insert(std::make_pair(i, std::shared_ptr<void>(ptr)));
}

template<typename T>
void
deserialize(Deserializer& d, std::unique_ptr<T>* p)
{
    new (p) std::unique_ptr<T>(new T(deserialize<T>(d)));
}

template <typename T>
void
deserialize(Deserializer& deserializer, std::shared_ptr<T>* p)
{
    deserializer.deserialize(p);
}

template <typename T>
void
deserialize(Deserializer& d, std::vector<T>* p)
{
    std::size_t n = deserialize<std::size_t>(d);
    std::vector<T> xs;
    xs.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        xs.push_back(deserialize<T>(d));
    }
    new (p) std::vector<T>(std::move(xs));
}

} // namespace Borges

#endif // BORGES_DESERIALIZER_H
