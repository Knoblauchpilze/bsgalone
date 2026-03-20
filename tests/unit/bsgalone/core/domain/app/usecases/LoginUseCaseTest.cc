
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

class MockSystemRepo : public ForManagingSystem
{
  public:
  MockSystemRepo()           = default;
  ~MockSystemRepo() override = default;

  MOCK_METHOD(System, findOneById, (const Uuid), (const, override));
  MOCK_METHOD(std::vector<System>, findAll, (), (const, override));
  MOCK_METHOD(void, save, (const System &), (override));
};

class MockPublisher : public ForPublishingPlayerMessage
{
  public:
  MockPublisher()           = default;
  ~MockPublisher() override = default;

  MOCK_METHOD(void, publishSystemList, (const Uuid, const std::vector<System> &), (override));
};
} // namespace

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, ThrowsWhenAccountRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockSystemRepo = std::make_shared<MockSystemRepo>();
      auto mockPublisher  = std::make_shared<MockPublisher>();
      LoginUseCase(nullptr, mockSystemRepo, mockPublisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, ThrowsWhenSystemRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockAccountRepo = std::make_shared<MockAccountRepo>();
      auto mockPublisher   = std::make_shared<MockPublisher>();
      LoginUseCase(mockAccountRepo, nullptr, mockPublisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, ThrowsWhenEventBusIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockAccountRepo = std::make_shared<MockAccountRepo>();
      auto mockSystemRepo  = std::make_shared<MockSystemRepo>();
      LoginUseCase(mockAccountRepo, mockSystemRepo, nullptr);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, FetchesAccount)
{
  auto mockAccountRepo = std::make_shared<MockAccountRepo>();
  auto mockSystemRepo  = std::make_shared<MockSystemRepo>();
  auto mockPublisher   = std::make_shared<MockPublisher>();
  LoginUseCase usecase(mockAccountRepo, mockSystemRepo, mockPublisher);

  ForExecutingLogin::Data data{.username = "player",
                               .password = "password",
                               .role     = GameRole::PILOT};

  EXPECT_CALL(*mockAccountRepo, findOneByName("player"))
    .Times(1)
    .WillOnce(Return(std::optional<Account>{}));

  usecase.performLogin(data);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, FetchesAllSystems)
{
  auto mockAccountRepo = std::make_shared<MockAccountRepo>();
  auto mockSystemRepo  = std::make_shared<MockSystemRepo>();
  auto mockPublisher   = std::make_shared<MockPublisher>();
  LoginUseCase usecase(mockAccountRepo, mockSystemRepo, mockPublisher);

  EXPECT_CALL(*mockSystemRepo, findAll()).Times(1).WillOnce(Return(std::vector<System>()));

  usecase.publishLoginData(Uuid{18});
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, ForwardsSystemsToPublisher)
{
  auto mockAccountRepo = std::make_shared<MockAccountRepo>();
  auto mockSystemRepo  = std::make_shared<MockSystemRepo>();
  auto mockPublisher   = std::make_shared<MockPublisher>();
  LoginUseCase usecase(mockAccountRepo, mockSystemRepo, mockPublisher);

  const std::vector<System> systems{System{.dbId = {17}}, System{.dbId = {19}}};
  ON_CALL(*mockSystemRepo, findAll()).WillByDefault(Return(systems));
  EXPECT_CALL(*mockPublisher, publishSystemList(Uuid{18}, systems)).Times(1);

  usecase.publishLoginData(Uuid{18});
}

} // namespace bsgalone::core
