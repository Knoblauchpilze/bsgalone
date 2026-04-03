
#include "SignupUseCase.hh"
#include "MockAccountRepository.hh"
#include "MockPlayerRepository.hh"
#include "PlayerLoginEvent.hh"
#include "TestGameEventPublisher.hh"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace test;
using namespace ::testing;

namespace bsgalone::core {

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SignupUseCase, ThrowsWhenAccountRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockPlayerRepo = std::make_shared<MockPlayerRepository>();
      auto publisher      = std::make_shared<TestGameEventPublisher>();
      SignupUseCase(nullptr, mockPlayerRepo, publisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SignupUseCase, ThrowsWhenPlayerRepositoryIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockAccountRepo = std::make_shared<MockAccountRepository>();
      auto publisher       = std::make_shared<TestGameEventPublisher>();
      SignupUseCase(mockAccountRepo, nullptr, publisher);
    }(),
    std::invalid_argument);
}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SignupUseCase, ThrowsWhenEventPublisherIsNull)
{
  EXPECT_THROW(
    []() {
      auto mockAccountRepo = std::make_shared<MockAccountRepository>();
      auto mockPlayerRepo  = std::make_shared<MockPlayerRepository>();
      SignupUseCase(mockAccountRepo, mockPlayerRepo, nullptr);
    }(),
    std::invalid_argument);
}

// TODO: Implement those tests
TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SignupUseCase,
     PublishesFailedSignupEventWhenAccountAlreadyExists)
{}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SignupUseCase,
     PublishesFailedSignupEventWhenAccountAlreadyHasAPlayer)
{}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SignupUseCase,
     PublishesSuccessfulSignupEventWhenSignupSucceed)
{}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SignupUseCase, RegistersResourcesForNewPlayer) {}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SignupUseCase, RegistersDefaultShipForNewColonialPlayer)
{}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SignupUseCase, RegistersDefaultShipForNewCylonPlayer) {}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SignupUseCase,
     RegistersDefaultSystemForNewColonialPlayer)
{}

TEST(Unit_Bsgalone_Core_Domain_App_Usecases_SignupUseCase, RegistersDefaultSystemForNewCylonPlayer)
{}

} // namespace bsgalone::core
