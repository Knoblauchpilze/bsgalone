
#pragma once

#include "CoreObject.hh"
#include "MessageType.hh"
#include <memory>

namespace bsgo {

class IMessage : public core::CoreObject
{
  public:
  IMessage(const std::string &name);
  ~IMessage() override = default;

  virtual auto type() const -> MessageType                          = 0;
  virtual auto serialize(std::ostream &out) const -> std::ostream & = 0;
  virtual bool deserialize(std::istream &in)                        = 0;

  virtual auto clone() const -> std::unique_ptr<IMessage> = 0;

  template<typename Message>
  auto as() -> Message &;

  template<typename Message>
  auto as() const -> const Message &;

  template<typename Message>
  bool isA() const;
};

using IMessagePtr = std::unique_ptr<IMessage>;

auto operator<<(std::ostream &out, const IMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, IMessage &message) -> std::istream &;

} // namespace bsgo

#include "IMessage.hxx"
