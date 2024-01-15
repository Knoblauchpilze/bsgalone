
#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgo {

class LootMessage : public AbstractMessage
{
  public:
  LootMessage(const Uuid &resourceId, const float amount);
  ~LootMessage() override = default;

  auto resourceId() const -> Uuid;
  auto amount() const -> float;

  auto operator<<(std::ostream &out) const -> std::ostream & override;
  auto operator>>(std::istream &in) -> std::istream & override;

  private:
  Uuid m_resourceId{};
  float m_amount{};
};

} // namespace bsgo
