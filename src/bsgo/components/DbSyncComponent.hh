

#pragma once

#include "SyncComponent.hh"

namespace bsgo {

class DbSyncComponent : public SyncComponent
{
  public:
  DbSyncComponent();
  ~DbSyncComponent() override = default;
};

using DbSyncComponentShPtr = std::shared_ptr<DbSyncComponent>;

} // namespace bsgo
