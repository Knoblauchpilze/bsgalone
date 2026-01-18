
#pragma once

#include "PGEApp.hh"

namespace bsgalone::client {

class App : public pge::PGEApp
{
  public:
  /// @brief - Create a new client application for the game. The app will respect
  /// the input properties and attempt to connect to the server listening on the
  /// specified port.
  /// @param desc - contains all the needed information to create the canvas needed
  /// @param serverPort - the port to use to connect to the game server
  /// by the app and set up base properties.
  App(const pge::AppDesc &desc, const int serverPort);

  /// @brief - Create a new client application for the game and performs the login
  /// for the specified user and password.
  /// @param desc - contains all the needed information to create the canvas needed
  /// @param serverPort - the port to use to connect to the game server
  /// @param userName - the name of the user to login with
  /// @param password - the password to use to login
  App(const pge::AppDesc &desc,
      const int serverPort,
      const std::optional<std::string> &userName,
      const std::optional<std::string> &password);

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
  int m_serverPort{};
  std::optional<std::string> m_userName{};
  std::optional<std::string> m_password{};
};

} // namespace bsgalone::client
