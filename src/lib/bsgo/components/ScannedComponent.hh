
#pragma once

#include "AbstractComponent.hh"

namespace bsgo {

class ScannedComponent : public AbstractComponent
{
  public:
  ScannedComponent();
  ~ScannedComponent() override = default;

  bool scanned() const;
  void scan();
  void reset();

  void update(const float elapsedSeconds) override;

  private:
  bool m_scanned{false};
};

using ScannedComponentShPtr = std::shared_ptr<ScannedComponent>;

} // namespace bsgo
