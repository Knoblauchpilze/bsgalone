
#pragma once

#include "CoreObject.hh"
#include "DatabaseEntityMapper.hh"
#include "Repositories.hh"

namespace bsgo {

class Coordinator;

class PlayerDataSource : public core::CoreObject
{
  public:
  PlayerDataSource(const Repositories &repositories);
  ~PlayerDataSource() override = default;

  void initialize(const Uuid systemDbId,
                  Coordinator &coordinator,
                  DatabaseEntityMapper &entityMapper) const;

  void registerPlayer(Coordinator &coordinator,
                      const Uuid playerDbId,
                      DatabaseEntityMapper &entityMapper) const;

  private:
  Repositories m_repositories{};
};

} // namespace bsgo
