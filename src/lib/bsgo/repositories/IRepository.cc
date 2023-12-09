
#include "IRepository.hh"

namespace bsgo {

IRepository::IRepository(const std::string &name)
  : utils::CoreObject(name)
{
  setService("repository");
}

void IRepository::initialize()
{
  /// TODO: Remove this.
}

} // namespace bsgo
