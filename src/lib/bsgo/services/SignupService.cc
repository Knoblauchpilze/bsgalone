
#include "SignupService.hh"

namespace bsgo {

SignupService::SignupService(const Repositories &repositories)
  : AbstractService("signup", repositories)
{}

} // namespace bsgo
