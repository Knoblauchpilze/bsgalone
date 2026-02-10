
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class AbstractPlayerMessage : public AbstractMessage
{
  public:
  ~AbstractPlayerMessage() override = default;

  auto getPlayerDbId() const -> Uuid;

  protected:
  AbstractPlayerMessage(const MessageType &type, const Uuid playerDbId);

  private:
  Uuid m_playerDbId{};
};

} // namespace bsgo
