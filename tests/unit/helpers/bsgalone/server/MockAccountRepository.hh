
#pragma once

#include "ForManagingAccount.hh"
#include <gmock/gmock.h>

namespace test {

class MockAccountRepository : public bsgalone::core::ForManagingAccount
{
  public:
  MockAccountRepository()           = default;
  ~MockAccountRepository() override = default;

  MOCK_METHOD(std::optional<bsgalone::core::Account>,
              findOneByName,
              (const std::string &),
              (const, override));
  MOCK_METHOD(bsgalone::core::Account, save, (bsgalone::core::Account), (const, override));
};

} // namespace test
