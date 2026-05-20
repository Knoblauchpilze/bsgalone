
#pragma once

#include "ForManagingSystem.hh"
#include <gmock/gmock.h>

namespace test {

class MockSystemRepository : public bsgalone::server::ForManagingSystem
{
  public:
  MockSystemRepository()           = default;
  ~MockSystemRepository() override = default;

  MOCK_METHOD(bsgalone::server::System,
              findOneById,
              (const bsgalone::core::Uuid),
              (const, override));
  MOCK_METHOD(std::vector<bsgalone::server::System>, findAll, (), (const, override));
  MOCK_METHOD(void, save, (const bsgalone::server::System &), (const, override));
};

} // namespace test
