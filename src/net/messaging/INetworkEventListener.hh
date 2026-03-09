
#pragma once

#include "IEventListener.hh"
#include "INetworkEvent.hh"
#include "NetworkEventType.hh"
#include <memory>

namespace net {

using INetworkEventListener    = messaging::IEventListener<NetworkEventType, INetworkEvent>;
using INetworkEventListenerPtr = std::unique_ptr<INetworkEventListener>;

} // namespace net
