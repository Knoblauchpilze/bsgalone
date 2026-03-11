
#include "SystemData.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

bool SystemData::operator==(const SystemData &rhs) const
{
  return dbId == rhs.dbId;
}

auto operator<<(std::ostream &out, const SystemData &data) -> std::ostream &
{
  ::core::serialize(out, data.dbId);
  ::core::serialize(out, data.name);
  ::core::serialize(out, data.position);

  return out;
}

bool operator>>(std::istream &in, SystemData &data)
{
  bool ok{true};

  ok &= ::core::deserialize(in, data.dbId);
  ok &= ::core::deserialize(in, data.name);
  ok &= ::core::deserialize(in, data.position);

  return ok;
}

} // namespace bsgalone::core
