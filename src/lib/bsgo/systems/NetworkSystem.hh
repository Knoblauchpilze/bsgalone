
#pragma once

#include "AbstractSystem.hh"

namespace bsgo {

class NetworkSystem : public AbstractSystem
{
  public:
  NetworkSystem();
  ~NetworkSystem() override = default;

  void updateEntity(Entity &entity,
                    Coordinator &coordinator,
                    const float elapsedSeconds) const override;
};

} // namespace bsgo
