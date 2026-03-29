
#pragma once

#include "RenderState.hh"
#include "Renderer.hh"
#include "RenderingPass.hh"
#include "Vector2d.hh"
#include <memory>

namespace bsgalone::client {

class IRenderer
{
  public:
  IRenderer()          = default;
  virtual ~IRenderer() = default;

  /// @brief - Prompts the renderer to load the resources needed to display its content.
  /// @param dimensions - the dimensions in pixels given to the renderer to draw
  /// @param texturesLoader - a common object to load textures. Allows to mutualize the
  /// loading of textures across the application
  virtual void loadResources(const pge::Vec2i &dimensions, pge::sprites::TexturePack &texturesLoader)
    = 0;

  /// @brief - Called each frame to give the renderer the opportunity to draw content
  /// on screen.
  /// @param engine - the rendering engine to use to draw content
  /// @param state - information about the state of the rendering: allows renderers to
  /// adapt to the user's actions and only display relevant content.
  /// @param pass - the type of drawing to perform
  virtual void render(pge::Renderer &engine,
                      const pge::RenderState &state,
                      const pge::RenderingPass pass) const
    = 0;
};

using IRendererPtr = std::unique_ptr<IRenderer>;

} // namespace bsgalone::client
