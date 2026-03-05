
#include "IRepository.hh"

namespace bsgo {

IRepository::IRepository(const std::string &name)
  : core::CoreObject(name)
{
  setService("repository");
}

} // namespace bsgo
