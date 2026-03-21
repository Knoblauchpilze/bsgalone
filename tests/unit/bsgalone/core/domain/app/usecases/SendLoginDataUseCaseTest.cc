
#include "SendLoginDataUseCase.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

namespace bsgalone::core {
namespace {
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

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SendLoginDataUseCase, ThrowsWhenSystemRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockPublisher = std::make_shared<MockPublisher>();
      SendLoginDataUseCase(nullptr, mockPublisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SendLoginDataUseCase, ThrowsWhenEventBusIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockSystemRepo = std::make_shared<MockSystemRepo>();
      SendLoginDataUseCase(mockSystemRepo, nullptr);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SendLoginDataUseCase, FetchesAllSystems)
{
  auto mockSystemRepo = std::make_shared<MockSystemRepo>();
  auto mockPublisher  = std::make_shared<MockPublisher>();
  SendLoginDataUseCase usecase(mockSystemRepo, mockPublisher);

  EXPECT_CALL(*mockSystemRepo, findAll()).Times(1).WillOnce(Return(std::vector<System>()));

  usecase.publishLoginData(Uuid{18});
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SendLoginDataUseCase, ForwardsSystemsToPublisher)
{
  auto mockSystemRepo = std::make_shared<MockSystemRepo>();
  auto mockPublisher  = std::make_shared<MockPublisher>();
  SendLoginDataUseCase usecase(mockSystemRepo, mockPublisher);

  const std::vector<System> systems{System{.dbId = {17}}, System{.dbId = {19}}};
  ON_CALL(*mockSystemRepo, findAll()).WillByDefault(Return(systems));
  EXPECT_CALL(*mockPublisher, publishSystemList(Uuid{18}, systems)).Times(1);

  usecase.publishLoginData(Uuid{18});
}

} // namespace bsgalone::core
