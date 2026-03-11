
#include "SystemTickData.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

bool SystemTickData::operator==(const SystemTickData &rhs) const
{
  return dbId == rhs.dbId;
}

auto operator<<(std::ostream &out, const SystemTickData &data) -> std::ostream &
{
  ::core::serialize(out, data.dbId);
  ::core::serialize(out, data.currentTick);
  ::core::serialize(out, data.step);

  return out;
}

bool operator>>(std::istream &in, SystemTickData &data)
{
  bool ok{true};

  ok &= ::core::deserialize(in, data.dbId);
  ok &= ::core::deserialize(in, data.currentTick);
  ok &= ::core::deserialize(in, data.step);

  return ok;
}

} // namespace bsgalone::core
