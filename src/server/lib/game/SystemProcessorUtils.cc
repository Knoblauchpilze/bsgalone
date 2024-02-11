
#include "SystemProcessorUtils.hh"

namespace bsgo {

auto convertToSystemProcessorMap(const std::vector<SystemProcessorShPtr> &systemProcessors)
  -> SystemProcessorMap
{
  SystemProcessorMap out{};

  for (const auto &system : systemProcessors)
  {
    const auto res = out.try_emplace(system->getSystemDbId(), system);
    if (!res.second)
    {
      throw std::invalid_argument("Failed to register duplicated system "
                                  + str(system->getSystemDbId()));
    }
  }

  return out;
}

} // namespace bsgo
