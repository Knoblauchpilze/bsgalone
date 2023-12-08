
#include "IService.hh"

namespace bsgo {

IService::IService(const std::string &name)
  : utils::CoreObject(name)
{
  setService("service");
}

} // namespace bsgo
