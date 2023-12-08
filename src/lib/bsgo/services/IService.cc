
#include "IService.hh"

namespace bsgo {

IService::IService(const std::string &name)
  : utils::CoreObject(name)
{
  setService("service");
}

bool IService::isReady() const noexcept
{
  return true;
}

} // namespace bsgo
