

#pragma once

#include "AbstractMessage.hh"
#include "Uuid.hh"

namespace bsgo {

enum class DockState
{
  STARTED,
  COMPLETED
};

class DockMessage : public AbstractMessage
{
  public:
  DockMessage(const Uuid &shipDbId,
              const Uuid &shipEntityId,
              const bool docking,
              const DockState &state);
  ~DockMessage() override = default;

  auto getShipDbId() const -> Uuid;
  auto getShipEntityId() const -> Uuid;
  bool isDocking() const;
  auto getDockState() const -> DockState;

  auto operator<<(std::ostream &out) const -> std::ostream & override;
  auto operator>>(std::istream &in) -> std::istream & override;

  private:
  Uuid m_shipDbId{};
  Uuid m_shipEntityId{};
  bool m_docking{false};
  DockState m_state{};
};

} // namespace bsgo
