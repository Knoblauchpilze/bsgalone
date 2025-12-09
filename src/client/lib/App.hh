
#pragma once

#include "Game.hh"
#include "PGEApp.hh"

namespace pge {

class App : public PGEApp
{
  public:
  /// @brief - Create a new client application for the game. The app will respect
  /// the input properties and attempt to connect to the server listening on the
  /// specified port.
  /// @param desc - contains all the needed information to create the canvas needed
  /// @param serverPort - the port to use to connect to the game server
  /// by the app and set up base properties.
  App(const AppDesc &desc, const int serverPort);

  /// @brief - Create a new client application for the game and performs the login
  /// for the specified user and password.
  /// @param desc - contains all the needed information to create the canvas needed
  /// @param serverPort - the port to use to connect to the game server
  /// @param userName - the name of the user to login with
  /// @param password - the password to use to login
  /// @param gameRole - the game role to use to login
  /// by the app and set up base properties.
  App(const AppDesc &desc,
      const int serverPort,
      const std::optional<std::string> &userName,
      const std::optional<std::string> &password,
      const std::optional<bsgo::GameRole> &gameRole);

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
  int m_serverPort{};
  std::optional<std::string> m_userName{};
  std::optional<std::string> m_password{};
  std::optional<bsgo::GameRole> m_gameRole{};
  GameShPtr m_game{nullptr};
};

} // namespace pge
