
#pragma once

#include "AbstractService.hh"
#include "Faction.hh"
#include <memory>
#include <optional>

namespace bsgalone::server {

class SignupService : public AbstractService
{
  public:
  SignupService(const core::Repositories &repositories);
  ~SignupService() override = default;

  auto trySignup(const std::string &name,
                 const std::string &password,
                 const core::Faction &faction) const -> std::optional<core::Player>;

  auto getPlayerSystemDbId(const core::Uuid playerDbId) const -> core::Uuid;

  private:
  auto registerAccount(const core::Account &account) const -> core::Uuid;
  auto registerPlayer(const core::Player &player) const -> core::Uuid;
  void registerResources(const core::Player &player) const;
  void registerShip(const core::Player &player) const;
};

using SignupServicePtr = std::unique_ptr<SignupService>;

} // namespace bsgalone::server
