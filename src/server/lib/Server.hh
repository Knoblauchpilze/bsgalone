

#pragma once

#include "Context.hh"
#include <atomic>
#include <core_utils/CoreObject.hh>

namespace bsgo {
class Server : public utils::CoreObject
{
  public:
  Server();
  ~Server() override = default;

  void run();
  void requestStop();

  private:
  net::Context m_context{};
  std::atomic_bool m_running{false};

  void setup();
  void activeRunLoop();
  void shutdown();
};
} // namespace bsgo
