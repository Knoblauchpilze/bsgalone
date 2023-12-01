
#include "UiTextField.hh"
#include <unordered_set>

namespace pge {

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
using KeyMapping = std::unordered_map<controls::keys::Keys, char>;

const KeyMapping KEYS_TO_TEXT_MAPPING = {
  {controls::keys::A, 'a'},      {controls::keys::B, 'b'},      {controls::keys::C, 'c'},
  {controls::keys::D, 'd'},      {controls::keys::E, 'e'},      {controls::keys::F, 'f'},
  {controls::keys::G, 'g'},      {controls::keys::H, 'h'},      {controls::keys::I, 'i'},
  {controls::keys::J, 'j'},      {controls::keys::K, 'k'},      {controls::keys::L, 'l'},
  {controls::keys::M, 'm'},      {controls::keys::N, 'n'},      {controls::keys::O, 'o'},
  {controls::keys::P, 'p'},      {controls::keys::Q, 'q'},      {controls::keys::R, 'r'},
  {controls::keys::S, 's'},      {controls::keys::T, 't'},      {controls::keys::U, 'u'},
  {controls::keys::V, 'v'},      {controls::keys::W, 'w'},      {controls::keys::X, 'x'},
  {controls::keys::Y, 'y'},      {controls::keys::Z, 'z'},      {controls::keys::K0, '0'},
  {controls::keys::K1, '1'},     {controls::keys::K2, '2'},     {controls::keys::K3, '3'},
  {controls::keys::K4, '4'},     {controls::keys::K5, '5'},     {controls::keys::K6, '6'},
  {controls::keys::K7, '7'},     {controls::keys::K8, '8'},     {controls::keys::K9, '9'},
  {controls::keys::NP0, '0'},    {controls::keys::NP1, '1'},    {controls::keys::NP2, '2'},
  {controls::keys::NP3, '3'},    {controls::keys::NP4, '4'},    {controls::keys::NP5, '5'},
  {controls::keys::NP6, '6'},    {controls::keys::NP7, '7'},    {controls::keys::NP8, '8'},
  {controls::keys::NP9, '9'},    {controls::keys::NP_MUL, '*'}, {controls::keys::NP_DIV, '/'},
  {controls::keys::NP_ADD, '+'}, {controls::keys::NP_SUB, '-'}, {controls::keys::NP_DECIMAL, '.'},
  {controls::keys::PERIOD, '.'}};

const KeyMapping KEYS_TO_TEXT_MAPPING_WITH_SHIFT
  = {{controls::keys::A, 'A'}, {controls::keys::B, 'B'}, {controls::keys::C, 'C'},
     {controls::keys::D, 'D'}, {controls::keys::E, 'E'}, {controls::keys::F, 'F'},
     {controls::keys::G, 'G'}, {controls::keys::H, 'H'}, {controls::keys::I, 'I'},
     {controls::keys::J, 'J'}, {controls::keys::K, 'K'}, {controls::keys::L, 'L'},
     {controls::keys::M, 'M'}, {controls::keys::N, 'N'}, {controls::keys::O, 'O'},
     {controls::keys::P, 'P'}, {controls::keys::Q, 'Q'}, {controls::keys::R, 'R'},
     {controls::keys::S, 'S'}, {controls::keys::T, 'T'}, {controls::keys::U, 'U'},
     {controls::keys::V, 'V'}, {controls::keys::W, 'W'}, {controls::keys::X, 'X'},
     {controls::keys::Y, 'Y'}, {controls::keys::Z, 'Z'}};

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

void UiTextField::updateInternalText(const controls::State &controls)
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

  if (controls.released(controls::keys::BACK) && !m_fullText.empty())
  {
    m_cursorPos = eraseOrTrim(m_fullText, m_cursorPos);
  }
  if (controls.released(controls::keys::DEL) && !m_fullText.empty())
  {
    deleteFrom(m_fullText, m_cursorPos);
  }
}

void UiTextField::updateCursorPosition(const controls::State &controls)
{
  const auto textLength = static_cast<int>(m_fullText.size());

  if (controls.released(controls::keys::HOME))
  {
    m_cursorPos = 0;
  }
  if (controls.released(controls::keys::LEFT))
  {
    m_cursorPos = std::max(0, m_cursorPos - 1);
  }
  if (controls.released(controls::keys::RIGHT))
  {
    m_cursorPos = std::min(textLength, m_cursorPos + 1);
  }
  if (controls.released(controls::keys::END))
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

} // namespace pge
