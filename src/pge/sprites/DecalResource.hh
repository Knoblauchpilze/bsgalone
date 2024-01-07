
#pragma once

#include "olcPixelGameEngine.h"
#include <memory>

namespace pge {

class DecalResource
{
  public:
  DecalResource(olc::Decal *decal);

  auto get() const -> olc::Decal *;

  auto w() const -> int;
  auto h() const -> int;

  private:
  struct DecalDeleter
  {
    void operator()(olc::Decal *decal) const noexcept;
  };

  using DecalPtr = std::unique_ptr<olc::Decal, DecalDeleter>;

  DecalPtr m_decal{nullptr};
};

using DecalResourcePtr = std::unique_ptr<DecalResource>;

} // namespace pge
