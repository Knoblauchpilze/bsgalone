

#pragma once

#include "AsioContext.hh"
#include "AsioServer.hh"
#include "CoreObject.hh"
#include "IEventBus.hh"
#include <atomic>
#include <condition_variable>
#include <mutex>

namespace bsgalone::server {
class Server : public core::CoreObject
{
  public:
  Server(net::details::AsioContext &context, const int port, net::IEventBusShPtr eventBus);
  ~Server() override = default;

  void run();
  void requestStop();

  private:
  net::details::AsioServerShPtr m_server{};

  std::atomic_bool m_running{false};
  std::mutex m_locker{};
  std::condition_variable m_notifier{};

  void activeRunLoop();
};

} // namespace bsgalone::server
