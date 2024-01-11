
#pragma once

#include "MessageType.hh"
#include <vector>

namespace net {

class Message
{
  public:
  Message(const MessageType type);
  ~Message() = default;

  auto type() const -> MessageType;
  auto size() const -> std::size_t;

  bool hasBody() const;

  private:
  MessageType m_type{};
  std::vector<char> m_body{};
};

} // namespace net