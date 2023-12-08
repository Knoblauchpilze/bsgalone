
#include "SignupService.hh"

namespace bsgo {

SignupService::SignupService(const Repositories &repositories)
  : AbstractService("signup", repositories)
{}

auto SignupService::trySignup(const std::string &name,
                              const std::string &password,
                              const Faction &faction) const -> std::optional<Uuid>
{
  const auto maybePlayer = m_repositories.playerRepository->findOneByName(name);
  if (maybePlayer)
  {
    warn("Player with name \"" + name + "\" already exists");
    return {};
  }

  Player player{.name = name, .password = password, .faction = faction};

  return m_repositories.playerRepository->save(player);
}

} // namespace bsgo
