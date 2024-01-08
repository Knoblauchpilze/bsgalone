
#pragma once

#include "olcPixelGameEngine.h"
#include <functional>
#include <optional>

namespace pge {

using UserCreateCallback  = std::function<bool()>;
using UserUpdateCallback  = std::function<bool(const float elapsedTime)>;
using UserDestroyCallback = std::function<bool()>;

struct WrapperCallbacks
{
  std::optional<UserCreateCallback> createCallback{};
  std::optional<UserUpdateCallback> updateCallback{};
  std::optional<UserDestroyCallback> destroyCallback{};
};

class PgeWrapper : public olc::PixelGameEngine
{
  public:
  PgeWrapper(const WrapperCallbacks &config);
  ~PgeWrapper() override = default;

  bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;
  bool OnUserDestroy() override;

  private:
  std::optional<UserCreateCallback> m_createCallback{};
  std::optional<UserUpdateCallback> m_updateCallback{};
  std::optional<UserDestroyCallback> m_destroyCallback{};
};

} // namespace pge
