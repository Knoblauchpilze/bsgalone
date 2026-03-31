
#include "ServerDataStore.hh"

namespace bsgalone::client {

ServerDataStore::ServerDataStore()
  : ::core::CoreObject("data")
{
  setService("store");
}

} // namespace bsgalone::client
