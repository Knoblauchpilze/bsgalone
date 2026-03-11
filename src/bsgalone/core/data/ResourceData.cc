
#include "ResourceData.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

bool ResourceData::operator==(const ResourceData &rhs) const
{
  return dbId == rhs.dbId;
}

auto operator<<(std::ostream &out, const ResourceData &data) -> std::ostream &
{
  ::core::serialize(out, data.dbId);
  ::core::serialize(out, data.name);

  return out;
}

bool operator>>(std::istream &in, ResourceData &data)
{
  bool ok{true};

  ok &= ::core::deserialize(in, data.dbId);
  ok &= ::core::deserialize(in, data.name);

  return ok;
}

} // namespace bsgalone::core
