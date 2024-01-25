
#pragma once

#include "NetworkMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class ScannedMessage : public NetworkMessage
{
  public:
  ScannedMessage();
  ScannedMessage(const Uuid &asteroidEntityId);
  ~ScannedMessage() override = default;

  auto asteroidEntityId() const -> Uuid;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  private:
  Uuid m_asteroidEntityId{};
};

} // namespace bsgo
