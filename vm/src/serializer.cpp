#include "serializer.h"

namespace Borges {

Serializer::Serializer(std::vector<double>& b) : buffer_(b)
{
}

void
Serializer::serialize(double x)
{
    buffer_.push_back(x);
}

void
serialize(Serializer& s, bool p)
{
    serialize(s, static_cast<double>(p));
}

void
serialize(Serializer& s, double x)
{
    s.serialize(x);
}

void
serialize(Serializer& s, std::size_t n)
{
    serialize(s, static_cast<double>(n));
}

} // namespace Borges
