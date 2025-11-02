
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class AiTargetReachedMessage : public NetworkMessage
{
  public:
  AiTargetReachedMessage();
  AiTargetReachedMessage(const Uuid shipDbId, const int targetIndex);
  ~AiTargetReachedMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getTargetIndex() const -> int;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_shipDbId{};
  int m_targetIndex{};
};

} // namespace bsgo
