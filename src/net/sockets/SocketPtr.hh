
#pragma once

#include <asio.hpp>
#include <memory>

namespace net {

using SocketShPtr = std::shared_ptr<asio::ip::tcp::socket>;

}
