
#pragma once

#include "ForManagingAccount.hh"
#include <gmock/gmock.h>

namespace test {

class MockAccountRepository : public bsgalone::server::ForManagingAccount
{
  public:
  MockAccountRepository()           = default;
  ~MockAccountRepository() override = default;

  MOCK_METHOD(std::optional<bsgalone::server::Account>,
              findOneByName,
              (const std::string &),
              (const, override));
  MOCK_METHOD(bsgalone::server::Account, save, (bsgalone::server::Account), (const, override));
};

} // namespace test
