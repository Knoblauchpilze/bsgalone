
#pragma once

#include "Connection.hh"
#include <asio/asio.hpp>
#include <atomic>
#include <core_utils/CoreObject.hh>
#include <memory>
#include <mutex>
#include <thread>

namespace net {

class Context : public utils::CoreObject
{
  public:
  Context();
  ~Context() override = default;

  auto get() -> asio::io_context &;

  auto createConnection(const std::string &url, const int port) -> ConnectionShPtr;

  void start();
  void stop();

  private:
  asio::io_context m_asioContext{};
  std::mutex m_asioContextLocker{};
  std::atomic_bool m_running{false};
  std::thread m_contextThread{};
};

using ContextPtr = std::unique_ptr<Context>;

} // namespace net
