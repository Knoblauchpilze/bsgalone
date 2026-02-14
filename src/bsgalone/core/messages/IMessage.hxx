
#pragma once

#include "IMessage.hh"

namespace bsgalone::core {

template<typename Message>
inline auto IMessage::as() -> Message &
{
  return dynamic_cast<Message &>(*this);
}

template<typename Message>
inline auto IMessage::as() const -> const Message &
{
  return dynamic_cast<const Message &>(*this);
}

template<typename Message>
inline bool IMessage::isA() const
{
  return dynamic_cast<const Message *>(this) != nullptr;
}

} // namespace bsgalone::core
