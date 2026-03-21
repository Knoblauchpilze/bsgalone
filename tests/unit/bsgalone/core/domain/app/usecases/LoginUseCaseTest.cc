
#include "LoginUseCase.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
class MockAccountRepo : public ForManagingAccount
{
  public:
  MockAccountRepo()           = default;
  ~MockAccountRepo() override = default;

  MOCK_METHOD(std::optional<Account>, findOneByName, (const std::string &), (const, override));
};
} // namespace

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, ThrowsWhenAccountRepositoryIsNull)
{
  EXPECT_THROW([]() { LoginUseCase(nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, FetchesAccount)
{
  auto mockAccountRepo = std::make_shared<MockAccountRepo>();
  LoginUseCase usecase(mockAccountRepo);

  ForExecutingLogin::Data data{.username = "player",
                               .password = "password",
                               .role     = GameRole::PILOT};

  EXPECT_CALL(*mockAccountRepo, findOneByName("player"))
    .Times(1)
    .WillOnce(Return(std::optional<Account>{}));

  usecase.performLogin(data);
}

} // namespace bsgalone::core
