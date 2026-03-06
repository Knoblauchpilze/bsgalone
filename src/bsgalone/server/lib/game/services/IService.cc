
#include "IService.hh"

namespace bsgalone::server {

IService::IService(const std::string &name)
  : ::core::CoreObject(name)
{
  setService("service");
}

} // namespace bsgalone::server
