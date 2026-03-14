
#include "SystemSerialization.hh"
#include "SerializationUtils.hh"

namespace bsgalone::core {

auto operator<<(std::ostream &out, const System &data) -> std::ostream &
{
  ::core::serialize(out, data.dbId);
  ::core::serialize(out, data.name);
  ::core::serialize(out, data.position);
  ::core::serialize(out, data.currentTick);
  ::core::serialize(out, data.step);

  return out;
}

bool operator>>(std::istream &in, System &data)
{
  bool ok{true};

  ok &= ::core::deserialize(in, data.dbId);
  ok &= ::core::deserialize(in, data.name);
  ok &= ::core::deserialize(in, data.position);
  ok &= ::core::deserialize(in, data.currentTick);
  ok &= ::core::deserialize(in, data.step);

  return ok;
}

} // namespace bsgalone::core
