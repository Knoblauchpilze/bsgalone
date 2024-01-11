
#pragma once

#include "Connection.hh"
#include <asio/asio.hpp>
#include <core_utils/CoreObject.hh>

namespace net {

class Context : public utils::CoreObject
{
  public:
  Context();
  ~Context() override = default;

  auto createConnection() -> ConnectionPtr;

  private:
  asio::io_context m_context{};
};

} // namespace net