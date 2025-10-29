
#include "SystemTickData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool SystemTickData::operator==(const SystemTickData &rhs) const
{
  return dbId == rhs.dbId;
}

auto SystemTickData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, currentTick);
  core::serialize(out, step);

  return out;
}

bool SystemTickData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, currentTick);
  ok &= core::deserialize(in, step);

  return ok;
}

} // namespace bsgo
