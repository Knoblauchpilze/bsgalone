
#pragma once

#include "IMessageQueue.hh"
#include "Uuid.hh"
#include <unordered_map>

namespace bsgo {

using SystemQueueMap = std::unordered_map<Uuid, IMessageQueueShPtr>;

}
