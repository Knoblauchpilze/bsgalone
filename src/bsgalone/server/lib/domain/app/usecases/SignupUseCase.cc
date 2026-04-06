
#include "SignupUseCase.hh"
#include "PlayerSignupEvent.hh"

namespace bsgalone::server {

SignupUseCase::SignupUseCase(ForManagingAccountShPtr accountRepo,
                             ForManagingPlayerShPtr playerRepo,
                             ForPublishingEventShPtr eventPublisher)
  : ::core::CoreObject("signup")
  , m_accountRepo(std::move(accountRepo))
  , m_playerRepo(std::move(playerRepo))
  , m_eventPublisher(std::move(eventPublisher))
{
  setService("consumer");

  if (m_accountRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null account repository");
  }
  if (m_playerRepo == nullptr)
  {
    throw std::invalid_argument("Expected non null player repository");
  }
  if (m_eventPublisher == nullptr)
  {
    throw std::invalid_argument("Expected non null event publisher");
  }
}

void SignupUseCase::performSignup(const SignupData &data)
{
  const auto maybePlayer = tryRegisterPlayer(data);

  auto event = std::make_unique<PlayerSignupEvent>(data.clientId);

  if (!maybePlayer)
  {
    m_eventPublisher->publishEvent(std::move(event));
    return;
  }

  event->setPlayerDbId(maybePlayer->dbId);
  event->setFaction(maybePlayer->faction);

  m_eventPublisher->publishEvent(std::move(event));
}

auto SignupUseCase::tryRegisterPlayer(const SignupData &data) const -> std::optional<Player>
{
  std::optional<Player> maybePlayer{};

  withSafetyNet(
    [&data, &maybePlayer, this]() {
      const auto account = registerAccount(data);
      maybePlayer        = registerPlayer(account, data);
    },
    "tryRegisterPlayer");

  return maybePlayer;
}

auto SignupUseCase::registerAccount(const SignupData &data) const -> Account
{
  Account account{
    .username = data.username,
    .password = data.password,
  };
  return m_accountRepo->save(account);
}

auto SignupUseCase::registerPlayer(const Account &account, const SignupData &data) const -> Player
{
  Player player{
    .account = account.dbId,
    .name    = data.username,
    .faction = data.faction,
    .role    = core::GameRole::PILOT,
  };
  return m_playerRepo->save(player);
}

} // namespace bsgalone::server
