
#pragma once

#include "CoreObject.hh"
#include <asio.hpp>
#include <atomic>
#include <memory>
#include <thread>

namespace net::details {

class AsioContext : public core::CoreObject
{
  public:
  AsioContext();
  ~AsioContext() override = default;

  auto get() -> asio::io_context &;

  void start();
  void stop();

  private:
  asio::io_context m_context{};

  std::atomic_bool m_running{false};
  std::thread m_contextThread{};

  void asyncRunningLoop();
};

using AsioContextPtr = std::unique_ptr<AsioContext>;

} // namespace net::details
