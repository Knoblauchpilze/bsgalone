
#pragma once

#include "PGEApp.hh"
#include "Screen.hh"
#include <unordered_map>

namespace bsgalone::client {

class App : public pge::PGEApp
{
  public:
  /// @brief - Create a new client application for the game and performs the login
  /// for the specified user and password.
  /// @param desc - contains all the needed information to create the canvas needed
  App(const pge::AppDesc &desc);

  ~App() override = default;

  protected:
  bool onFrame(const float elapsedSeconds) override;
  void onInputs(const pge::controls::State &controls, pge::CoordinateFrame &frame) override;

  void loadResources(const pge::Vec2i &screenDims, pge::Renderer &engine) override;
  void cleanResources() override;

  void drawDecal(const pge::RenderState &state) override;
  void draw(const pge::RenderState &state) override;
  void drawUi(const pge::RenderState &state) override;
  void drawDebug(const pge::RenderState &state, const pge::Vec2f &mouseScreenPos) override;

  private:
  /// @brief - Defines the current screen selected in this game. Updated when
  /// the user takes action to change it.
  Screen m_screen{Screen::LOGIN};
};

} // namespace bsgalone::client
