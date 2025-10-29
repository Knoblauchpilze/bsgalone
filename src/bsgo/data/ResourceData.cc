
#include "ResourceData.hh"
#include "SerializationUtils.hh"

namespace bsgo {

bool ResourceData::operator==(const ResourceData &rhs) const
{
  return dbId == rhs.dbId;
}

auto ResourceData::serialize(std::ostream &out) const -> std::ostream &
{
  core::serialize(out, dbId);
  core::serialize(out, name);

  return out;
}

bool ResourceData::deserialize(std::istream &in)
{
  bool ok{true};

  ok &= core::deserialize(in, dbId);
  ok &= core::deserialize(in, name);

  return ok;
}

} // namespace bsgo
