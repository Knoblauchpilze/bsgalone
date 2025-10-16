
#pragma once

#include "AbstractComponent.hh"
#include "Uuid.hh"

namespace bsgo {

class DbComponent : public AbstractComponent
{
  public:
  DbComponent(const Uuid dbId);
  ~DbComponent() override = default;

  auto dbId() const noexcept -> Uuid;

  void update(const chrono::TickData &data) override;

  private:
  Uuid m_dbId{};
};

using DbComponentShPtr = std::shared_ptr<DbComponent>;

} // namespace bsgo
