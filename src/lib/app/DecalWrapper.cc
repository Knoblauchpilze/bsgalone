
#include "DecalWrapper.hh"

namespace pge {

void DecalDeleter::operator()(olc::Decal *decal) const noexcept
{
  if (!decal)
  {
    return;
  }

  if (nullptr != decal->sprite)
  {
    delete decal->sprite;
    decal->sprite = nullptr;
  }

  delete decal;
}

} // namespace pge
