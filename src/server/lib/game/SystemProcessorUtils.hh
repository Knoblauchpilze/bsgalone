
#pragma once

#include "SystemProcessor.hh"
#include <vector>

namespace bsgo {

auto convertToSystemProcessorMap(const std::vector<SystemProcessorShPtr> &systemProcessors)
  -> SystemProcessorMap;

} // namespace bsgo
