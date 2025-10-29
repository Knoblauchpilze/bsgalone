
#include "SystemData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool SystemData::operator==(const SystemData &rhs) const
{
  return dbId == rhs.dbId;
}

auto SystemData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, name);
  core::serialize(out, position);

  return out;
}

bool SystemData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, name);
  ok &= core::deserialize(in, position);

  return ok;
}

} // namespace bsgo
