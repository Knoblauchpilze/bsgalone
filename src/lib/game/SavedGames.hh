
#pragma once

#include "Menu.hh"
#include <core_utils/CoreObject.hh>
#include <core_utils/Signal.hh>
#include <string>
#include <unordered_set>
#include <vector>

namespace pge {

class SavedGames : public utils::CoreObject
{
  public:
  /// @brief - Creates a new class handling the save games with the input properties.
  /// @param count - the number of games to display.
  /// @param dir - the name of the directory where games are stored.
  /// @param ext - the extension of the files to consider.
  SavedGames(unsigned count, const std::string &dir, const std::string &ext) noexcept;

  /// @brief - Generate the layout of this menu and attach all the menu that are
  /// needed to the input parent.
  /// @param menu - the parent to attach the saved games to.
  void generate(MenuShPtr menu);

  /// @brief - Used to update the list of saved games. It is typically used in case
  /// the load game menu is being displayed to ensure that we have up to date info
  /// in it.
  void refresh();

  /// @brief - Used to genertae a new name for a saved game in the directory which
  /// is not used yet.
  /// @return - a new name for the file.
  std::string generateNewName() const noexcept;

  public:
  /// @brief - Used to update the display menus representing the list of saved games
  /// available for loading. This assumes the list of games is up to date and will
  /// use the current index to display all subsequent games in the limit of the
  /// allowed number per page. It will also handle the de/activation of the navigation
  /// buttons if needed.
  void update();

  private:
  /// @brief - Convenience define for a list of file names.
  using Files = std::unordered_set<std::string>;

  /// @brief - The directory where saved games are stored.
  std::string m_dir;

  /// @brief - The extension of the saved games files.
  std::string m_ext;

  /// @brief - The list of saved games as listed in the directory where games are
  /// stored.
  std::vector<std::string> m_saves;

  /// @brief - The index of the first element displayed in the load game screen.
  /// Starts at `0` and can range until all the games are displayed (assuming
  /// we're displaying a certain number per page).
  unsigned m_index;

  /// @brief - The number of saved games displayed per page.
  unsigned m_gamesPerPage;

  /// @brief - The menus allowing to display the saved games names.
  std::vector<MenuShPtr> m_games;

  /// @brief - The menu representing the previous page option.
  MenuShPtr m_previous;

  /// @brief - The menu representing the next page option.
  MenuShPtr m_next;

  /// @brief - The current index reached when requesting new names for saved files.
  mutable unsigned m_fileIndex;

  /// @brief - The list of files currently available in the directory. Allows to
  /// not overwrite any file when generating a new name for a saved game.
  mutable Files m_existingFiles;

  public:
  /// @brief - Signal emitted whenever a new saved game is selected by the user.
  /// The parameter corresponds to the full path to the saved game file that was
  /// selected.
  utils::Signal<const std::string &> onSavedGameSelected;
};

using SavedGamesShPtr = std::shared_ptr<SavedGames>;
} // namespace pge
