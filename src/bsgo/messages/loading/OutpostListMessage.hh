
#pragma once

#include "NetworkMessage.hh"
#include "OutpostData.hh"
#include "Uuid.hh"

namespace bsgo {

class OutpostListMessage : public NetworkMessage
{
  public:
  OutpostListMessage();
  OutpostListMessage(const Uuid systemDbId, const std::vector<OutpostData> &outpostsData);
  ~OutpostListMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getOutpostsData() const -> const std::vector<OutpostData> &;

  auto serialize(std::ostream &out) const -> std::ostream & override;
  bool deserialize(std::istream &in) override;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<OutpostData> m_outpostsData{};
};

} // namespace bsgo
