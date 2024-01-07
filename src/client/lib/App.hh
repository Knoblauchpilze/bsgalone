
#pragma once

#include "Game.hh"
#include "PGEApp.hh"
#include "Renderer.hh"

namespace pge {

class App : public PGEApp
{
  public:
  /// @brief - Create a new default pixel game engine app.
  /// @param desc - contains all the needed information to create the canvas needed
  /// by the app and set up base properties.
  App(const AppDesc &desc);

  ~App() = default;

  protected:
  bool onFrame(float fElapsed) override;
  void onInputs(const controls::State &controls, CoordinateFrame &cf) override;

  void loadResources() override;
  void cleanResources() override;

  void drawDecal(const RenderState &res) override;
  void draw(const RenderState &res) override;
  void drawUI(const RenderState &res) override;
  void drawDebug(const RenderState &res) override;

  void renderCursor(const RenderState &res);

  private:
  GameShPtr m_game{nullptr};
  RendererPtr m_renderer{nullptr};
};

} // namespace pge
