
#pragma once

#include "NetworkMessage.hh"
#include "OutpostData.hh"
#include "Uuid.hh"

namespace bsgalone::core {

class OutpostListMessage : public NetworkMessage
{
  public:
  OutpostListMessage();
  OutpostListMessage(const Uuid systemDbId, const std::vector<OutpostData> &outpostsData);
  ~OutpostListMessage() override = default;

  auto getSystemDbId() const -> Uuid;
  auto getOutpostsData() const -> const std::vector<OutpostData> &;

  auto clone() const -> IMessagePtr override;

  private:
  Uuid m_systemDbId{};
  std::vector<OutpostData> m_outpostsData{};

  friend auto operator<<(std::ostream &out, const OutpostListMessage &message) -> std::ostream &;
  friend auto operator>>(std::istream &in, OutpostListMessage &message) -> std::istream &;
};

auto operator<<(std::ostream &out, const OutpostListMessage &message) -> std::ostream &;
auto operator>>(std::istream &in, OutpostListMessage &message) -> std::istream &;

} // namespace bsgalone::core
