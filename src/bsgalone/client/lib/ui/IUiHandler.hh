
#pragma once

#include "RenderState.hh"
#include "Renderer.hh"
#include "UserInputData.hh"
#include <memory>

namespace bsgalone::client {

class IUiHandler
{
  public:
  IUiHandler()          = default;
  virtual ~IUiHandler() = default;

  /// @brief - Initializes all UI widgets, menus and text resources needed by the handler.
  /// This is expected to be called once when application resources are loaded.
  /// @param width - the viewport width in pixels.
  /// @param height - the viewport height in pixels.
  /// @param texturesLoader - shared texture loader used to build UI assets.
  virtual void initializeMenus(const int width,
                               const int height,
                               pge::sprites::TexturePack &texturesLoader)
    = 0;

  /// @brief - Processes one frame of user input and records UI-triggered actions.
  /// Implementations typically inspect buttons, text fields and shortcuts to update
  /// the provided input data with high-level actions to execute.
  /// @param inputData - frame-local user input and action accumulator.
  /// @return `true` when the UI consumed input relevant to gameplay interactions.
  virtual bool processUserInput(ui::UserInputData &inputData) = 0;

  /// @brief - Renders UI overlays for the current frame (menus, prompts, status text).
  /// @param engine - renderer used to draw widgets and text.
  virtual void render(pge::Renderer &engine) const = 0;

  /// @brief - Updates transient UI state that evolves over time (timers, animations,
  /// visibility toggles, pending messages) before rendering.
  virtual void updateUi() = 0;
};

using IUiHandlerPtr = std::unique_ptr<IUiHandler>;

} // namespace bsgalone::client
