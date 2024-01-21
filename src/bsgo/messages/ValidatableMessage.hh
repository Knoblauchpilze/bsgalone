
#pragma once

#include "AbstractMessage.hh"

namespace bsgo {

class ValidatableMessage : public AbstractMessage
{
  public:
  ValidatableMessage(const MessageType &type);
  ValidatableMessage(const MessageType &type, const bool validated);
  ~ValidatableMessage() override = default;

  bool validated() const;

  private:
  bool m_validated{false};
};

} // namespace bsgo
