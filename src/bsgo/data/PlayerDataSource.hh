
#pragma once

#include "DatabaseEntityMapper.hh"
#include "Repositories.hh"
#include <core_utils/CoreObject.hh>

namespace bsgo {

class Coordinator;

class PlayerDataSource : public utils::CoreObject
{
  public:
  PlayerDataSource(const Repositories &repositories);
  ~PlayerDataSource() override = default;

  void initialize(const Uuid systemDbId,
                  Coordinator &coordinator,
                  DatabaseEntityMapper &entityMapper) const;

  private:
  Repositories m_repositories{};

  void registerPlayer(Coordinator &coordinator,
                      const Uuid playerDbId,
                      DatabaseEntityMapper &entityMapper) const;
  void registerResources(Coordinator &coordinator,
                         const Uuid playerEntityId,
                         const Uuid playerDbId) const;
};

} // namespace bsgo
