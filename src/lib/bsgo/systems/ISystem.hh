
#pragma once

namespace bsgo {

class ISystem
{
  public:
  virtual ~ISystem() = default;

  virtual void update(const float elapsedSeconds) = 0;
};

} // namespace bsgo
