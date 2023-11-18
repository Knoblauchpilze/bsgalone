
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace pge {

class Game;

using GameCallback = std::function<void(Game &)>;

class Action : public utils::CoreObject
{
  public:
  Action(const GameCallback &callback);
  ~Action() override = default;

  void apply(Game &g) const;

  private:
  GameCallback m_callback{};
};

using ActionPtr   = std::unique_ptr<Action>;
using ActionShPtr = std::shared_ptr<Action>;

} // namespace pge
