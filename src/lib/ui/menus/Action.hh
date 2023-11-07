
#pragma once

#include "Game.hh"
#include <core_utils/CoreObject.hh>
#include <memory>

namespace pge {

// Forward declaration the game class onto which the actions will be applied.
class Game;

namespace action {

/// @brief - Defines a common function called whenever the action is triggered.
/// This allows to specialize the menu class easily when not much is needed.
using Process = std::function<void(Game &g)>;

} // namespace action

class Action : public utils::CoreObject
{
  public:
  /// @brief - Create a new empty action.
  /// @param name - the name of the action (for logging purposes).
  Action(const action::Process &cb, const std::string &name = "action");

  ~Action() override = default;

  /// @brief - Interface method allowing to perform an action. Nothing is known
  /// of the actual process performed by the action and we don't need to.
  /// The goal is to provide a base interface allowing menus to trigger some
  /// changes in the game. Inheriting classes should redefine this method based
  /// on their effect.
  /// @param g - the game onto which the action should be applied.
  virtual void apply(Game &g) const;

  /// @brief - Assign a new process to be triggered in case this action is applied.
  /// @param cb - the new process to trigger when this action is applied.
  void setProcess(const action::Process &cb);

  private:
  /// @brief - The callback to trigger when this action is triggered.
  action::Process m_callback;
};

using ActionShPtr = std::shared_ptr<Action>;
} // namespace pge
