
#pragma once

#include "CoreObject.hh"
#include "DatabaseEntityMapper.hh"
#include "PlayerData.hh"
#include "Repositories.hh"
#include <optional>

namespace bsgo {

class Coordinator;

class PlayerDataSource : public core::CoreObject
{
  public:
  PlayerDataSource();
  PlayerDataSource(const Repositories &repositories);
  ~PlayerDataSource() override = default;

  void initialize(const Uuid systemDbId,
                  Coordinator &coordinator,
                  DatabaseEntityMapper &entityMapper) const;

  void registerPlayer(Coordinator &coordinator,
                      const PlayerData &data,
                      DatabaseEntityMapper &entityMapper) const;

  void registerPlayer(Coordinator &coordinator,
                      const Uuid playerDbId,
                      DatabaseEntityMapper &entityMapper) const;

  private:
  std::optional<Repositories> m_repositories{};
};

} // namespace bsgo
