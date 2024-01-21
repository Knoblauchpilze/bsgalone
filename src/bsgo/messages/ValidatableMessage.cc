
#include "ValidatableMessage.hh"

namespace bsgo {

ValidatableMessage::ValidatableMessage(const MessageType &type)
  : ValidatableMessage(type, false)
{}

ValidatableMessage::ValidatableMessage(const MessageType &type, const bool validated)
  : AbstractMessage(type)
  , m_validated(validated)
{}

bool ValidatableMessage::validated() const
{
  return m_validated;
}

void ValidatableMessage::validate(const bool validate)
{
  m_validated = validate;
}

} // namespace bsgo
