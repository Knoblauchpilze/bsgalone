
#include "UiTextField.hh"
#include <unordered_set>

namespace ui {

UiTextField::UiTextField(const TextFieldConfig &config,
                         const BackgroundConfig &bg,
                         const TextConfig &text)
  : UiTextMenu(
    MenuConfig{
      .pos                       = config.pos,
      .dims                      = config.dims,
      .layout                    = MenuLayout::HORIZONTAL,
      .visible                   = config.visible,
      .highlightable             = false,
      .propagateEventsToChildren = false,
      .highlightCallback         = {},
      .clickCallback             = {},
      .lostFocusCallback         = {},
      .gameClickCallback         = config.gameClickCallback,
    },
    bg,
    text)
  , m_cursorPos(static_cast<int>(text.text.size()))
  , m_fullText(text.text)
{
  setClickCallback([this]() { m_editing = true; });
  setLostFocusCallback([this]() { m_editing = false; });
}

auto UiTextField::getText() const noexcept -> std::string
{
  return m_fullText;
}

bool UiTextField::processUserInput(UserInputData &inputData)
{
  if (m_editing)
  {
    updateInternalText(inputData.controls);
    updateCursorPosition(inputData.controls);
  }
  generateDisplayedText();

  return UiTextMenu::processUserInput(inputData);
}

namespace {
using KeyMapping = std::unordered_map<pge::controls::keys::Keys, char>;

const KeyMapping KEYS_TO_TEXT_MAPPING
  = {{pge::controls::keys::A, 'a'},          {pge::controls::keys::B, 'b'},
     {pge::controls::keys::C, 'c'},          {pge::controls::keys::D, 'd'},
     {pge::controls::keys::E, 'e'},          {pge::controls::keys::F, 'f'},
     {pge::controls::keys::G, 'g'},          {pge::controls::keys::H, 'h'},
     {pge::controls::keys::I, 'i'},          {pge::controls::keys::J, 'j'},
     {pge::controls::keys::K, 'k'},          {pge::controls::keys::L, 'l'},
     {pge::controls::keys::M, 'm'},          {pge::controls::keys::N, 'n'},
     {pge::controls::keys::O, 'o'},          {pge::controls::keys::P, 'p'},
     {pge::controls::keys::Q, 'q'},          {pge::controls::keys::R, 'r'},
     {pge::controls::keys::S, 's'},          {pge::controls::keys::T, 't'},
     {pge::controls::keys::U, 'u'},          {pge::controls::keys::V, 'v'},
     {pge::controls::keys::W, 'w'},          {pge::controls::keys::X, 'x'},
     {pge::controls::keys::Y, 'y'},          {pge::controls::keys::Z, 'z'},
     {pge::controls::keys::K0, '0'},         {pge::controls::keys::K1, '1'},
     {pge::controls::keys::K2, '2'},         {pge::controls::keys::K3, '3'},
     {pge::controls::keys::K4, '4'},         {pge::controls::keys::K5, '5'},
     {pge::controls::keys::K6, '6'},         {pge::controls::keys::K7, '7'},
     {pge::controls::keys::K8, '8'},         {pge::controls::keys::K9, '9'},
     {pge::controls::keys::NP0, '0'},        {pge::controls::keys::NP1, '1'},
     {pge::controls::keys::NP2, '2'},        {pge::controls::keys::NP3, '3'},
     {pge::controls::keys::NP4, '4'},        {pge::controls::keys::NP5, '5'},
     {pge::controls::keys::NP6, '6'},        {pge::controls::keys::NP7, '7'},
     {pge::controls::keys::NP8, '8'},        {pge::controls::keys::NP9, '9'},
     {pge::controls::keys::NP_MUL, '*'},     {pge::controls::keys::NP_DIV, '/'},
     {pge::controls::keys::NP_ADD, '+'},     {pge::controls::keys::NP_SUB, '-'},
     {pge::controls::keys::NP_DECIMAL, '.'}, {pge::controls::keys::PERIOD, '.'}};

const KeyMapping KEYS_TO_TEXT_MAPPING_WITH_SHIFT
  = {{pge::controls::keys::A, 'A'}, {pge::controls::keys::B, 'B'}, {pge::controls::keys::C, 'C'},
     {pge::controls::keys::D, 'D'}, {pge::controls::keys::E, 'E'}, {pge::controls::keys::F, 'F'},
     {pge::controls::keys::G, 'G'}, {pge::controls::keys::H, 'H'}, {pge::controls::keys::I, 'I'},
     {pge::controls::keys::J, 'J'}, {pge::controls::keys::K, 'K'}, {pge::controls::keys::L, 'L'},
     {pge::controls::keys::M, 'M'}, {pge::controls::keys::N, 'N'}, {pge::controls::keys::O, 'O'},
     {pge::controls::keys::P, 'P'}, {pge::controls::keys::Q, 'Q'}, {pge::controls::keys::R, 'R'},
     {pge::controls::keys::S, 'S'}, {pge::controls::keys::T, 'T'}, {pge::controls::keys::U, 'U'},
     {pge::controls::keys::V, 'V'}, {pge::controls::keys::W, 'W'}, {pge::controls::keys::X, 'X'},
     {pge::controls::keys::Y, 'Y'}, {pge::controls::keys::Z, 'Z'}};

auto addOrAppend(std::string &str, const int size, const char letter)
{
  const auto textSize = static_cast<int>(str.size());
  if (size >= textSize)
  {
    str += letter;
  }
  else
  {
    str.insert(str.begin() + size, letter);
  }
}

constexpr auto CHARACTERS_TO_REMOVE = 1;

auto eraseOrTrim(std::string &str, const int size) -> int
{
  const auto textSize = static_cast<int>(str.size());
  if (size >= textSize)
  {
    str.pop_back();
    return size - 1;
  }
  else if (size >= 1)
  {
    str.erase(size - 1, CHARACTERS_TO_REMOVE);
    return size - 1;
  }

  return size;
}

void deleteFrom(std::string &str, const int size)
{
  const auto textSize = static_cast<int>(str.size());
  if (size < textSize)
  {
    str.erase(size, CHARACTERS_TO_REMOVE);
  }
}
} // namespace

void UiTextField::updateInternalText(const pge::controls::State &controls)
{
  const KeyMapping *keysMapping = controls.shift ? &KEYS_TO_TEXT_MAPPING_WITH_SHIFT
                                                 : &KEYS_TO_TEXT_MAPPING;

  for (const auto &[key, mapping] : *keysMapping)
  {
    if (controls.released(key))
    {
      addOrAppend(m_fullText, m_cursorPos, mapping);
      ++m_cursorPos;
    }
  }

  if (controls.released(pge::controls::keys::BACK) && !m_fullText.empty())
  {
    m_cursorPos = eraseOrTrim(m_fullText, m_cursorPos);
  }
  if (controls.released(pge::controls::keys::DEL) && !m_fullText.empty())
  {
    deleteFrom(m_fullText, m_cursorPos);
  }
}

void UiTextField::updateCursorPosition(const pge::controls::State &controls)
{
  const auto textLength = static_cast<int>(m_fullText.size());

  if (controls.released(pge::controls::keys::HOME))
  {
    m_cursorPos = 0;
  }
  if (controls.released(pge::controls::keys::LEFT))
  {
    m_cursorPos = std::max(0, m_cursorPos - 1);
  }
  if (controls.released(pge::controls::keys::RIGHT))
  {
    m_cursorPos = std::min(textLength, m_cursorPos + 1);
  }
  if (controls.released(pge::controls::keys::END))
  {
    m_cursorPos = textLength;
  }
}

void UiTextField::generateDisplayedText()
{
  constexpr auto CURSOR_CHARACTER = '|';

  auto text = m_fullText;
  if (m_editing)
  {
    addOrAppend(text, m_cursorPos, CURSOR_CHARACTER);
  }
  setText(text);
}

} // namespace ui
