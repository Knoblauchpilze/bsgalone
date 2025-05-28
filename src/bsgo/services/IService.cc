
#include "IService.hh"

namespace bsgo {

IService::IService(const std::string &name)
  : core::CoreObject(name)
{
  setService("service");
}

} // namespace bsgo
