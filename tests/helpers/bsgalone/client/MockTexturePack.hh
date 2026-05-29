
#pragma once

#include "ITexturePack.hh"
#include <gmock/gmock.h>

namespace test {

class MockTexturePack : public pge::sprites::ITexturePack
{
  public:
  MockTexturePack()           = default;
  ~MockTexturePack() override = default;

  MOCK_METHOD(pge::DecalResourcePtr, loadDecal, (const std::string &), (override));
  MOCK_METHOD(pge::sprites::PackId, registerPack, (const pge::sprites::PackDesc &pack), (override));

  MOCK_METHOD(void,
              draw,
              (const pge::sprites::Sprite &, const pge::Vec2f &, const pge::Vec2f &),
              (const, override));
  using Corners = std::array<pge::Vec2f, 4>;
  MOCK_METHOD(void, draw, (const pge::sprites::Sprite &, const Corners &), (const, override));
  MOCK_METHOD(void,
              draw,
              (const pge::sprites::Sprite &, const pge::Vec2f &, const pge::Vec2f &, const float),
              (const, override));
};

} // namespace test
