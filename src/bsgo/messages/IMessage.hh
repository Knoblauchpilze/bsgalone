
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

  virtual auto type() const -> MessageType                          = 0;
  virtual auto serialize(std::ostream &out) const -> std::ostream & = 0;
  virtual bool deserialize(std::istream &in)                        = 0;

  template<typename Message>
  auto as() const -> const Message &;
};

using IMessagePtr = std::unique_ptr<IMessage>;

auto operator<<(std::ostream &out, const IMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, IMessage &message) -> std::istream &;

} // namespace bsgo

#include "IMessage.hxx"
