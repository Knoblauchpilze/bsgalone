
#pragma once

#include <asio.hpp>
#include <string>

namespace net::details {

auto str(const asio::ip::tcp::endpoint &endpoint) -> std::string;

}
