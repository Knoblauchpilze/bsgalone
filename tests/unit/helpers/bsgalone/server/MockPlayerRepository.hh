
#pragma once

#include "ForManagingPlayer.hh"
#include <gmock/gmock.h>

namespace test {

class MockPlayerRepository : public bsgalone::server::ForManagingPlayer
{
  public:
  MockPlayerRepository()           = default;
  ~MockPlayerRepository() override = default;

  MOCK_METHOD(bsgalone::server::Player,
              findOneById,
              (const bsgalone::core::Uuid),
              (const, override));
  MOCK_METHOD(bsgalone::server::Player,
              findOneByAccount,
              (const bsgalone::core::Uuid),
              (const, override));
  MOCK_METHOD(bsgalone::server::Player, save, (bsgalone::server::Player), (override));
};

} // namespace test
