
#include "LoginUseCase.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// using namespace test;
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

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, ThrowsWhenSystemRepositoryIsNull)
{
  auto mockPublisher = std::make_shared<MockPublisher>();
  EXPECT_THROW([&mockPublisher]() { LoginUseCase(nullptr, mockPublisher); }(),
               std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, ThrowsWhenEventBusIsNull)
{
  auto mockRepo = std::make_shared<MockSystemRepo>();
  EXPECT_THROW([&mockRepo]() { LoginUseCase(mockRepo, nullptr); }(), std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, FetchesAllSystems)
{
  auto mockRepo      = std::make_shared<MockSystemRepo>();
  auto mockPublisher = std::make_shared<MockPublisher>();
  LoginUseCase usecase(mockRepo, mockPublisher);

  EXPECT_CALL(*mockRepo, findAll()).Times(1).WillOnce(Return(std::vector<System>()));

  usecase.publishLoginData(Uuid{18});
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_LoginUseCase, ForwardsSystemsToPublisher)
{
  auto mockRepo      = std::make_shared<MockSystemRepo>();
  auto mockPublisher = std::make_shared<MockPublisher>();
  LoginUseCase usecase(mockRepo, mockPublisher);

  const std::vector<System> systems{System{.dbId = {17}}, System{.dbId = {19}}};
  ON_CALL(*mockRepo, findAll()).WillByDefault(Return(systems));
  EXPECT_CALL(*mockPublisher, publishSystemList(Uuid{18}, systems)).Times(1);

  usecase.publishLoginData(Uuid{18});
}

} // namespace bsgalone::core
