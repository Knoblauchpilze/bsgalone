
#pragma once

#include <memory>

namespace bsgalone::client {

class IDataStore
{
  public:
  IDataStore()          = default;
  virtual ~IDataStore() = default;
};

using IDataStoreShPtr = std::shared_ptr<IDataStore>;

} // namespace bsgalone::client
