
#pragma once

#include "IMessage.hh"

namespace bsgo {

template<typename Message>
inline auto IMessage::as() const -> const Message &
{
  return dynamic_cast<const Message &>(*this);
}

} // namespace bsgo
