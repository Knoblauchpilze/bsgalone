
#include "LoginService.hh"

namespace bsgalone::server {

LoginService::LoginService(const core::Repositories &repositories)
  : AbstractService("login", repositories)
{}

auto LoginService::tryLogin(const LoginData &data) const -> std::optional<core::Uuid>
{
  const auto maybeAccount = m_repositories.accountRepository->findOneByName(data.name);
  if (!maybeAccount)
  {
    warn("No account with name \"" + data.name + "\"");
    return {};
  }
  if (maybeAccount->password != data.password)
  {
    warn("Wrong password for player \"" + data.name + "\"",
         "Expected " + maybeAccount->password + " but got " + data.password);
    return {};
  }

  const auto player = m_repositories.playerRepository->findOneByAccount(maybeAccount->dbId);
  core::PlayerRole playerRole{
    .player = player.id,
    .role   = data.role,
  };
  m_repositories.playerRoleRepository->save(playerRole);

  info("Player " + data.name + " logged in with role " + core::str(data.role));

  return player.id;
}

auto LoginService::getPlayerSystemDbId(const core::Uuid playerDbId) const -> core::Uuid
{
  return m_repositories.playerRepository->findSystemByPlayer(playerDbId);
}

} // namespace bsgalone::server
