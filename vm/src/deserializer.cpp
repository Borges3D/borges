#include "deserializer.h"
#include <cassert>
#include <stdexcept>

namespace Borges {

Deserializer::Deserializer(std::size_t n, const double* p)
    : buffer_size_(n), buffer_data_(p), buffer_index_(0)
{
    assert(buffer_data_);
}

void
Deserializer::deserialize(double* p)
{
    if (buffer_index_ == buffer_size_) {
        throw std::runtime_error("buffer underflow");
    }
    double x = buffer_data_[buffer_index_];
    ++buffer_index_;
    *p = x;
}

void
deserialize(Deserializer& d, bool* p)
{
    *p = deserialize<double>(d);
}

void
deserialize(Deserializer& d, double* p)
{
    d.deserialize(p);
}

void
deserialize(Deserializer& d, std::size_t* p)
{
    *p = deserialize<double>(d);
}

} // namespace Borges
