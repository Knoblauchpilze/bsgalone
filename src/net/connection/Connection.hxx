
#pragma once

#include "Connection.hh"
#include <ostream>

namespace net {

template<typename T>
void Connection::send(const T &message)
{
  std::ostringstream out{};
  out << message;

  const auto &rawMessage = out.str();
  std::vector<char> data(rawMessage.begin(), rawMessage.end());

  auto messageToSend = std::make_unique<MessageToSend>(std::move(data));
  registerMessageToSend(std::move(messageToSend));
}

} // namespace net
