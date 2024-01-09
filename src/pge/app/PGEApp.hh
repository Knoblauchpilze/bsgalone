
#pragma once

#include "AppDesc.hh"
#include "Color.hh"
#include "Controls.hh"
#include "CoordinateFrame.hh"
#include "FramerateControl.hh"
#include "RenderState.hh"
#include "Renderer.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
#include <optional>

namespace olc {
class PixelGameEngine;
}

namespace pge {

class PGEApp : public utils::CoreObject
{
  public:
  /// @brief - Create a new default pixel game engine app.
  /// @param desc - contains all the needed information to create the canvas needed
  /// by the app and set up base properties.
  PGEApp(const AppDesc &desc);

  ~PGEApp() override;

  /// @brief - Call this method to start a blocking call to the main loop of
  /// the application. This method will only return when the user requested
  /// to exit the application.
  void run();

  protected:
  /// @brief - Convenience define refering to a drawing layer.
  enum class Layer
  {
    DRAW,
    DECAL,
    UI,
    DEBUG
  };

  bool isFirstFrame() const noexcept;
  bool hasDebug() const noexcept;
  bool hasUi() const noexcept;

  /// @brief - Used to assign a certain tint to the layer defined by the input
  /// descriptor.
  /// @param layer - the layer for which a tint should be assigned.
  /// @param tint - the tint to associate to the layer.
  void setLayerTint(const Layer &layer, const Color &tint);

  /// @brief - Interface method allowing inheriting classes to get a chance to
  /// load resources needed for display. This method is guaranteed to be called
  /// before the first call to `draw` is issued.
  /// @param screenDims - the dimensions of the rendering canvas in pixels.
  /// @param engine - utility object to load graphic resources.
  virtual void loadResources(const Vec2i &screenDims, Renderer &engine) = 0;

  /// @brief - Interface method allowing inheriting classes to be notified when
  /// the app is going to be destroyed so that resources can be cleaned.
  /// The default implementation does nothing.
  virtual void cleanResources() = 0;

  /// @brief - Interface method to display the main content of the app. This
  /// method only handles the decal objects and is called first during the
  /// rendering step. It means that the content will be overriden by all UI
  /// and debug info.
  /// @param state - the resources to help the drawing.
  virtual void drawDecal(const RenderState &state) = 0;

  /// @brief - Interface method to display the main content of the app. This
  /// method is called to draw the non-decal instances of the content and is
  /// triggered right after the `drawDecal` one. It will still get overriden
  /// by UI and debug.
  /// @param state - the resources that can be drawn.
  virtual void draw(const RenderState &state) = 0;

  /// @brief - Interface method allowing to draw the UI of the application.
  /// This regroups menu and all needed elements that are not game elements.
  /// @param state - the resources that can be drawn.
  virtual void drawUi(const RenderState &state) = 0;

  /// @brief - Interface method allowing inheriting classes to perform their
  /// own drawing calls to show debug info.
  /// @param state - the resources that can be drawn.
  /// @param mouseScreenPos - the position of the mouse in screen coordinates.
  virtual void drawDebug(const RenderState &state, const Vec2f &mouseScreenPos) = 0;

  /// @brief - Interface method called at each frame to give inheriting classes
  /// an occasion to process the app logic.
  /// The return value indicates whether or not the game loop should be stopped.
  /// @param fElapsed - the duration in seconds of the last frame.
  ///  @return - `true` if the game loop should continue and `false` if the app
  /// should exit.
  virtual bool onFrame(float fElapsed) = 0;

  /// @brief - Interface method called at each frame when the inputs should be
  /// handled.
  /// @param controls - the current state of the controls.
  /// @param frame - the coordinate frame associated to this app.
  virtual void onInputs(const controls::State &controls, CoordinateFrame &frame) = 0;

  private:
  /// @brief - The pixel game engine used to run the application.
  std::unique_ptr<olc::PixelGameEngine> m_engine{nullptr};

  /// @brief - A wrapper for the rendering engine.
  RendererPtr m_renderer{nullptr};

  /// @brief - The index representing the main layer for this app. Given how the
  /// pixel game engine is designed we display layers with the highest order first
  /// and then in descending order.
  /// As we want the debug and UI layers to be on top of the base layer, we need
  /// to give it the largest index so that it is displayed first, and then the
  /// rest on top.
  uint32_t m_decalLayer{};

  //// @brief - The index representing the main layer but where only non-decal
  /// objects can be drawn. This layer will be displayed on top of the `m_mLayer`
  /// so it limits a bit how we can intertwine the items in the rendering.
  uint32_t m_drawLayer{};

  /// @brief - The index of the layer allowing to display debug information. This
  /// layer will be assigned to the default layer created by the pixel game engine.
  /// It is needed in order to initialize the last of the layers (and thus the one
  /// that will be set on top of all the others) with meaningful data.
  uint32_t m_debugLayer{};

  //// @brief - A layer used to represent all the UI elements of the application
  /// (menu, etc).
  uint32_t m_uiLayer{};

  /// @brief - Used to determine whether debug display is needed for this app.
  bool m_debugOn{true};

  /// @brief - Similar to the `m_debugOn` but controls whether the UI is active or
  /// not.
  bool m_uiOn{true};

  /// @brief - A map to keep track of the state of the controls to be transmitted
  /// to the world's entities for the simulation.
  controls::State m_controls{};

  /// @brief - Boolean allowing to display logs only on the first frame. Or do any
  /// other process a single time upon rendering the first frame.
  bool m_first{true};

  /// @brief - Whether or not panning and zooming is allowed in this app.
  bool m_fixedFrame;

  /// @brief - Holds an object allowing to convert between the various coordinate
  /// frames handled by the app. It handles conversion between tiles coordinate and
  /// screen coordinates and conversely.
  CoordinateFramePtr m_frame;

  /// @brief - Allows to control the framerate of the application. In case its
  /// value is not assigned, the app will run as fast as possible, otherwise it
  /// will try to maintain the maximum desired fps.
  std::optional<FramerateControl> m_fpsControl{};

  /// @brief - Performs the initialization of the engine to make it suits our needs.
  /// @param appName - the name of the app.
  /// @param dims - a vector describing the dimensions of the canvas for this app in
  /// pixels.
  /// @param screenPixToEnginePixRatio - the ratio of a pixel in the app compared to
  /// a pixel on screen. If this value is set set to `2` it means that each pixel in
  /// the app's canvas will be 2x2 pixels on screen.
  void initialize(const std::string &appName,
                  const Vec2i &dims,
                  const Vec2i &enginePixToScreenPixRatio);

  bool onCreate();
  bool onUpdate(const float elapsedSeconds);
  bool onDestroy();

  /// @brief - Used to keep track of the changes in the input
  /// that were processed during a frame. It helps determining
  /// whether some unique processes need to be triggered, such
  /// as cleaning of rendering layers that will not be updated
  /// anymore.
  struct InputChanges
  {
    bool quit{false};
    bool debugLayerToggled{false};
  };

  /// @brief - Used to perform the necessary update based on the controls that the
  /// user might have used in the game.
  // @return - a state describing the changes processed in this method. It includes
  /// any exit request of the user and changes to the UI.
  auto handleInputs() -> InputChanges;
};

} // namespace pge
