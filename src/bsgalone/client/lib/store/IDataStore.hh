
#pragma once

#include "GameRole.hh"
#include "Uuid.hh"
#include <memory>

namespace bsgalone::client {

class IDataStore
{
  public:
  IDataStore()          = default;
  virtual ~IDataStore() = default;

  virtual void onPlayerLoggedIn(const core::Uuid playerDbId,
                                const core::Uuid systemDbId,
                                const core::GameRole role)
    = 0;
};

using IDataStoreShPtr = std::shared_ptr<IDataStore>;

} // namespace bsgalone::client
