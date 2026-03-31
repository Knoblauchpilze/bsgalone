
#pragma once

#include "CoreObject.hh"
#include "IDataStore.hh"

namespace bsgalone::client {

class ServerDataStore : public IDataStore, public ::core::CoreObject
{
  public:
  ServerDataStore();
  ~ServerDataStore() override = default;

  private:
};

} // namespace bsgalone::client
