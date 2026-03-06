
#include "IRepository.hh"

namespace bsgalone::core {

IRepository::IRepository(const std::string &name)
  : ::core::CoreObject(name)
{
  setService("repository");
}

} // namespace bsgalone::core
