
#pragma once

#include <asio.hpp>
#include <string>
#include <vector>

namespace test {

auto readFrom(asio::ip::tcp::socket &socket, const std::size_t length) -> std::string;
void writeTo(asio::ip::tcp::socket &socket, const std::vector<char> &data);

} // namespace test
