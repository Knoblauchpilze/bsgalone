
#pragma once

#include "GameScreen.hh"
#include "IRenderer.hh"
#include "PGEApp.hh"
#include <unordered_map>

namespace bsgalone::client {

class GameApp : public pge::PGEApp
{
  public:
  GameApp(const pge::AppDesc &desc, const int serverPort);
  ~GameApp() override = default;

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
  int m_serverPort{};

  /// @brief - Defines the current screen selected in this game. Updated whenever
  /// the user takes action to change it.
  GameScreen screen{GameScreen::LOGIN};

  std::unordered_map<GameScreen, IRendererPtr> m_renderers{};
};

} // namespace bsgalone::client
