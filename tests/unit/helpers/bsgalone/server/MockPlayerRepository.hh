
#pragma once

#include "ForManagingPlayer.hh"
#include <gmock/gmock.h>

namespace test {

class MockPlayerRepository : public bsgalone::core::ForManagingPlayer
{
  public:
  MockPlayerRepository()           = default;
  ~MockPlayerRepository() override = default;

  MOCK_METHOD(bsgalone::core::Player, findOneById, (const bsgalone::core::Uuid), (const, override));
  MOCK_METHOD(bsgalone::core::Player,
              findOneByAccount,
              (const bsgalone::core::Uuid),
              (const, override));
  MOCK_METHOD(bsgalone::core::Player, save, (bsgalone::core::Player), (override));
};

} // namespace test
