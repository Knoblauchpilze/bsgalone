
#pragma once

#include "NetworkMessage.hh"

namespace bsgo {

class ValidatableMessage : public NetworkMessage
{
  public:
  ValidatableMessage(const MessageType &type);
  ValidatableMessage(const MessageType &type, const bool validated);
  ~ValidatableMessage() override = default;

  bool validated() const;
  void validate(const bool validate = true);

  protected:
  bool m_validated{false};
};

} // namespace bsgo
