
#pragma once

#include "IMessageQueue.hh"
#include "Uuid.hh"
#include <unordered_map>

namespace bsgalone::server {

using SystemQueueMap = std::unordered_map<core::Uuid, core::IMessageQueueShPtr>;

}
