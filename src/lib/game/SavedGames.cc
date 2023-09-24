
#include "SavedGames.hh"
#include <filesystem>

namespace {

pge::MenuShPtr generateGameEntry(const std::string &text,
                                 const olc::Pixel &bgColor,
                                 const olc::Pixel &textColor,
                                 const olc::Pixel &textHColor,
                                 const std::string &name)
{
  pge::menu::BackgroundDesc bg = pge::menu::newColoredBackground(bgColor);
  bg.hColor                    = olc::GREY;

  pge::menu::MenuContentDesc fd = pge::menu::newTextContent(text,
                                                            textColor,
                                                            pge::menu::Alignment::Center);
  fd.hColor                     = textHColor;

  pge::MenuShPtr m = std::make_shared<pge::Menu>(olc::vi2d(),
                                                 olc::vi2d(),
                                                 name,
                                                 bg,
                                                 fd,
                                                 pge::menu::Layout::Horizontal,
                                                 true,
                                                 false);

  return m;
}

} // namespace

namespace pge {

// Convenience using to shorten the usage of the filesystem
// data types when loading the saved games list.
using DirIt = std::filesystem::directory_iterator;

SavedGames::SavedGames(unsigned count, const std::string &dir, const std::string &ext) noexcept
  : utils::CoreObject("games")
  ,

  m_dir(dir)
  , m_ext(ext)
  ,

  m_saves()
  , m_index(0u)
  , m_gamesPerPage(count)
  ,

  m_games()
  , m_previous()
  , m_next()
  ,

  m_fileIndex(0u)
  , m_existingFiles()
{
  setService("saves");
}

void SavedGames::generate(MenuShPtr menu)
{
  olc::vi2d dims;

  // Generate previous page button.
  m_previous = generateGameEntry("Previous page",
                                 olc::VERY_DARK_CORNFLOWER_BLUE,
                                 olc::GREY,
                                 olc::BLACK,
                                 "previous");
  m_previous->setEnabled(false);

  m_previous->setSimpleAction([this](Game & /*g*/) {
    // Move to the previous page if possible.
    if (m_index > 0u)
    {
      m_index -= std::min(m_index, m_gamesPerPage);
      update();
    }
  });

  // Create menus for each line of the saved game screen.
  for (unsigned id = 0u; id < m_gamesPerPage; ++id)
  {
    MenuShPtr m = generateGameEntry("",
                                    olc::DARK_CORNFLOWER_BLUE,
                                    olc::GREY,
                                    olc::BLACK,
                                    "game" + std::to_string(id));
    m->setSimpleAction([this, m](Game & /*g*/) {
      // Concatenate the save directory path to the name
      // of the game so that we can readily path it to
      // other processes.
      std::string fullPath = m_dir + "/" + m->getText() + "." + m_ext;
      onSavedGameSelected.safeEmit("saved game selected", fullPath);
    });
    m->setEnabled(false);

    m_games.push_back(m);
  }

  // Generate next page button.
  m_next = generateGameEntry("Next page",
                             olc::VERY_DARK_CORNFLOWER_BLUE,
                             olc::GREY,
                             olc::BLACK,
                             "next");
  m_next->setEnabled(false);

  m_next->setSimpleAction([this](Game & /*g*/) {
    // Move to the next page if possible.
    if (m_index + m_gamesPerPage < m_saves.size())
    {
      m_index += m_gamesPerPage;
      update();
    }
  });

  // Register menu to the parent.
  menu->addMenu(m_previous);
  for (unsigned id = 0u; id < m_games.size(); ++id)
  {
    menu->addMenu(m_games[id]);
  }
  menu->addMenu(m_next);
}

void SavedGames::refresh()
{
  // Refresh the list of games that can be loaded by
  // re-reading the content of the save directory.
  // Note that we do not update the displayed values
  // within this method.
  DirIt end;
  DirIt it(m_dir);

  m_saves.clear();
  m_existingFiles.clear();

  log("Scanning directory \"" + m_dir + "\" for saved games", utils::Level::Debug);

  for (; it != end; ++it)
  {
    std::filesystem::directory_entry sg = *it;

    // Only keep the base path in the display: the
    // path to the save directory would just clutter
    // the output.
    std::string path = sg.path();
    std::string name = path.substr(m_dir.size() + 1u);

    // In any case, register this file.
    m_existingFiles.insert(path);

    // Also, only keep files matching the extension
    // provided to this object.
    std::size_t p = name.find_last_of('.');
    if (p != std::string::npos)
    {
      std::string ext = name.substr(p + 1u);
      if (ext != m_ext)
      {
        continue;
      }

      name = name.substr(0, p);
    }

    if (name.empty())
    {
      log("Failed to interpret saved game \"" + path + "\"", utils::Level::Error);
      continue;
    }

    m_saves.push_back(name);
  }

  // Sort the games in alphabetical order to ease
  // finding a particular game.
  std::sort(m_saves.begin(), m_saves.end());

  // Reset the index.
  m_index = 0u;

  // Update the display.
  update();
}

std::string SavedGames::generateNewName() const noexcept
{
  // Loop until we find a file name which does not
  // exist yet in the directory.
  std::string out = m_dir + "/save_" + std::to_string(m_fileIndex) + "." + m_ext;

  while (m_existingFiles.count(out) > 0)
  {
    ++m_fileIndex;
    out = m_dir + "/save_" + std::to_string(m_fileIndex) + "." + m_ext;
  }

  m_existingFiles.insert(out);

  return out;
}

void SavedGames::update()
{
  // Update the text of the display menus with
  // the name of the games starting from the one
  // pointed at by the virtual cursor.
  unsigned max = std::min(static_cast<unsigned>(m_saves.size()) - m_index, m_gamesPerPage);

  unsigned id = 0u;
  for (; id < max; ++id)
  {
    m_games[id]->setText(m_saves[m_index + id]);
    m_games[id]->setEnabled(true);
  }

  // Fill the rest of the lines with blank spaces.
  if (id < m_gamesPerPage)
  {
    for (; id < m_gamesPerPage; ++id)
    {
      m_games[id]->setText("");
      m_games[id]->setEnabled(false);
    }
  }

  // Update the next/previous page buttons.
  m_previous->setEnabled(m_index > 0u);
  m_next->setEnabled(m_index + m_gamesPerPage < m_saves.size());
}

} // namespace pge
