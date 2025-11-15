
#include "LoginService.hh"

namespace bsgo {

LoginService::LoginService(const Repositories &repositories)
  : AbstractService("login", repositories)
{}

auto LoginService::tryLogin(const std::string &name, const std::string &password) const
  -> std::optional<Uuid>
{
  const auto maybeAccount = m_repositories.accountRepository->findOneByName(name);
  if (!maybeAccount)
  {
    warn("No account with name \"" + name + "\"");
    return {};
  }
  if (maybeAccount->password != password)
  {
    warn("Wrong password for player \"" + name + "\"",
         "Expected " + maybeAccount->password + " but got " + password);
    return {};
  }

  const auto player = m_repositories.playerRepository->findOneByAccount(maybeAccount->id);

  return player.id;
}

auto LoginService::getPlayerSystemDbId(const Uuid playerDbId) const -> Uuid
{
  return m_repositories.playerRepository->findSystemByPlayer(playerDbId);
}

} // namespace bsgo
