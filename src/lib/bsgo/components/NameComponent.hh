
#pragma once

#include "AbstractComponent.hh"

namespace bsgo {

class NameComponent : public AbstractComponent
{
  public:
  NameComponent(const std::string &name);
  ~NameComponent() override = default;

  auto name() const noexcept -> std::string;

  void update(const float elapsedSeconds) override;

  private:
  std::string m_name{};
};

using NameComponentShPtr = std::shared_ptr<NameComponent>;

} // namespace bsgo
