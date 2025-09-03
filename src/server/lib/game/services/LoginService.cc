
#include "LoginService.hh"

namespace bsgo {

LoginService::LoginService(const Repositories &repositories)
  : AbstractService("login", repositories)
{}

auto LoginService::tryLogin(const std::string &name, const std::string &password) const
  -> std::optional<Uuid>
{
  const auto maybePlayer = m_repositories.playerRepository->findOneByName(name);
  if (!maybePlayer)
  {
    warn("No player with name \"" + name + "\"");
    return {};
  }
  if (maybePlayer->password != password)
  {
    warn("Wrong password for player \"" + name + "\"",
         "Expected " + maybePlayer->password + " but got " + password);
    return {};
  }

  return maybePlayer->id;
}

auto LoginService::getPlayerSystemDbId(const Uuid playerDbId) const -> Uuid
{
  return m_repositories.playerRepository->findSystemByPlayer(playerDbId);
}

} // namespace bsgo
