#include "type.h"
#include "deserializer.h"
#include "serializer.h"

namespace Borges {

void
serialize(Serializer& s, Type t)
{
    serialize(s, static_cast<std::size_t>(t));
}

void
deserialize(Deserializer& d, Type* p)
{
    *p = static_cast<Type>(deserialize<std::size_t>(d));
}

} // namespace Borges
