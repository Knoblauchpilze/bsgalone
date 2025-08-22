
#include "PlayerResourceData.hh"

namespace bsgo {

bool PlayerResourceData::operator==(const PlayerResourceData &rhs) const
{
  return dbId == rhs.dbId;
}

auto fromDbPlayerResource(const PlayerResource &playerResource) -> PlayerResourceData
{
  return PlayerResourceData{
    .dbId   = playerResource.resource,
    .name   = playerResource.name,
    .amount = playerResource.amount,
  };
}

} // namespace bsgo
