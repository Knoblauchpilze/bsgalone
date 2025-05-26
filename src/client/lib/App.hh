
#pragma once

#include "Game.hh"
#include "PGEApp.hh"

namespace pge {

class App : public PGEApp
{
  public:
  /// @brief - Create a new default pixel game engine app.
  /// @param desc - contains all the needed information to create the canvas needed
  /// @param serverPort - the port to use to connect to the game server
  /// by the app and set up base properties.
  App(const AppDesc &desc, const int serverPort);

  ~App() override = default;

  protected:
  bool onFrame(const float elapsedSeconds) override;
  void onInputs(const controls::State &controls, CoordinateFrame &frame) override;

  void loadResources(const Vec2i &screenDims, Renderer &engine) override;
  void cleanResources() override;

  void drawDecal(const RenderState &state) override;
  void draw(const RenderState &state) override;
  void drawUi(const RenderState &state) override;
  void drawDebug(const RenderState &state, const Vec2f &mouseScreenPos) override;

  private:
  int m_serverPort;
  GameShPtr m_game{nullptr};
};

} // namespace pge
