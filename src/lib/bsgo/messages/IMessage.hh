
#pragma once

#include "MessageType.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class IMessage : public utils::CoreObject
{
  public:
  IMessage(const std::string &name);
  ~IMessage() override = default;

  virtual auto type() const -> MessageType = 0;
};

using IMessagePtr = std::unique_ptr<IMessage>;

} // namespace bsgo
