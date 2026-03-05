
#include "LoginService.hh"

namespace bsgo {

LoginService::LoginService(const Repositories &repositories)
  : AbstractService("login", repositories)
{}

auto LoginService::tryLogin(const LoginData &data) const -> std::optional<Uuid>
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

  const auto player = m_repositories.playerRepository->findOneByAccount(maybeAccount->id);
  PlayerRole playerRole{
    .player = player.id,
    .role   = data.role,
  };
  m_repositories.playerRoleRepository->save(playerRole);

  info("Player " + data.name + " logged in with role " + str(data.role));

  return player.id;
}

auto LoginService::getPlayerSystemDbId(const Uuid playerDbId) const -> Uuid
{
  return m_repositories.playerRepository->findSystemByPlayer(playerDbId);
}

} // namespace bsgo
