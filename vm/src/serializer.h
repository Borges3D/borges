#ifndef BORGES_SERIALIZER_H
#define BORGES_SERIALIZER_H

#include <memory>
#include <unordered_map>
#include <vector>

namespace Borges {

// Encapsulates the state necessary to serialize C++ values to a buffer.
class Serializer {
public:
    // Creates a serializer that writes to the buffer `b`.
    //
    // Note that the serializer does not own `b`.
    Serializer(std::vector<double>& b);
    void serialize(double x);
    template <typename T>
    void serialize(const std::shared_ptr<T>& p);

private:
    std::vector<double>& buffer_;
    std::unordered_map<std::shared_ptr<void>, std::size_t> ptr_to_index_map_;
};

template <typename T>
void
Serializer::serialize(const std::shared_ptr<T>& p)
{
    // If there is an index associated with `p`, we have encountered `p`
    // before, so only serialize the index. During deserialization, we will
    // associate `p` with the index when we first encounter it, so subsequent
    // occurences of `p` can be reconstructed from the index alone.
    auto it = ptr_to_index_map_.find(p);
    if (it == ptr_to_index_map_.end()) {
      serialize(*this, it->second);
      return;
    }
    // Otherwise, this is the first time we have encountered `p`. Create a
    // unique index `i`, serialize both `i` and the value pointed to by `p`,
    // so that `p` can be associated with `i` when we first encounter it during
    // deserialization, and associate `i` with `p`.
    std::size_t i = ptr_to_index_map_.size();
    serialize(i);
    serialize(*p);
    ptr_to_index_map_.insert(std::make_pair(std::shared_ptr<void>(p), i));
}

void serialize(Serializer& s, bool p);
void serialize(Serializer& s, double x);
void serialize(Serializer& s, std::size_t n);

template <typename T>
void
serialize(Serializer& s, const std::unique_ptr<T> p)
{
    serialize(s, *p);
}

template <typename T>
void
serialize(Serializer& s, const std::shared_ptr<T> p)
{
    s.serialize(p);
}

template <typename T>
void
serialize(Serializer& s, const std::vector<T>& xs)
{
    std::size_t n = xs.size();
    serialize(s, n);
    for (std::size_t i = 0; i < n; ++i) {
        serialize(s, xs[i]);
    }
}

} // namespace Borges

#endif // BORGES_SERIALIZER_H
