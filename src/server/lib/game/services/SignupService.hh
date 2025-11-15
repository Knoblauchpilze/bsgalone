
#pragma once

#include "AbstractService.hh"
#include "Faction.hh"
#include <memory>
#include <optional>

namespace bsgo {

class SignupService : public AbstractService
{
  public:
  SignupService(const Repositories &repositories);
  ~SignupService() override = default;

  auto trySignup(const std::string &name, const std::string &password, const Faction &faction) const
    -> std::optional<Player>;

  auto getPlayerSystemDbId(const Uuid playerDbId) const -> Uuid;

  private:
  auto registerAccount(const Account &account) const -> Uuid;
  auto registerPlayer(const Player &player) const -> Uuid;
  void registerResources(const Player &player) const;
  void registerShip(const Player &player) const;
};

using SignupServicePtr   = std::unique_ptr<SignupService>;
using SignupServiceShPtr = std::shared_ptr<SignupService>;

} // namespace bsgo
