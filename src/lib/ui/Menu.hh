
#pragma once

#include "Action.hh"
#include "BackgroundDesc.hh"
#include "Controls.hh"
#include "MenuContentDesc.hh"
#include "olcEngine.hh"
#include <core_utils/CoreObject.hh>
#include <memory>
#include <vector>

namespace pge {

/// Forward declaration to be able to use shared pointers on
/// menu elements right away.
class Menu;
using MenuShPtr = std::shared_ptr<Menu>;

namespace menu {

//// @brief - Define a direction for a menu, or in general a layout for
/// children items.
enum class Layout
{
  Horizontal,
  Vertical
};

/// @brief - Convenience struct defining what can happen while processing
/// inputs. It is mostly used internally to detect update of children but
/// can also be interpreted by external callers if needed.
struct InputHandle
{
  bool relevant;
  bool selected;
};

/// @brief - Defines a common function called whenever the menu is clicked
/// upon. This allows to specialize the menu class easily when not much is
/// needed.
using RegisterAction = std::function<void(std::vector<ActionShPtr> &)>;

} // namespace menu

class Menu : public utils::CoreObject
{
  public:
  /// @brief - Create a new menu with the specified dimensions.
  /// @param pos - the position of the menu in the parent app.
  /// @param size - the dimensions of the menu.
  /// @param name - the name of the menu (for logging purposes).
  /// @param bg - the description of the background for the menu.
  /// @param fg - the description of the content for the menu.
  /// @param layout - the layout for this menu: will define how children are
  /// represented in the menu.
  /// @param clickable - indicates whether this menu can be clicked.
  /// @param selectable - indicates whether this menu can be selected.
  /// @param parent - the parent menu for this element. Specify `null` in case
  /// the menu is the root of the subsystem.
  Menu(const olc::vi2d &pos,
       const olc::vi2d &size,
       const std::string &name,
       const menu::BackgroundDesc &bg,
       const menu::MenuContentDesc &fg,
       const menu::Layout &layout = menu::Layout::Horizontal,
       bool clickable             = true,
       bool selectable            = true,
       Menu *parent               = nullptr);

  ~Menu();

  /// @brief - Whether or not the menu is visible.
  /// @return - `true` if the menu is visible.
  bool visible() const noexcept;

  /// @brief - Used to define whether this menu is visible. Changes will only
  /// be applied in the next call to `render` and the display should be cleared
  /// beforehand by external processes.
  /// @param visible - `true` if this menu is visible.
  void setVisible(bool visible) noexcept;

  /// @brief - Defines whether or not this menu support click on it. If set to
  /// `false` the menu will not react whenever the user clicks on it.
  /// @param click - `true` in case clicks are supported.
  void setClickable(bool click) noexcept;

  /// @brief - Defines whether or not this menu support selection. If set to
  /// `false` the menu will not react whenever the user attemps to select it
  /// by clicking. This will notably prevent to change the visual representation
  /// to use the highlighted colors.
  /// @param select - `true` in case selecion is supported.
  void setSelectable(bool select) noexcept;

  /// @brief - Used to turn this menu either active or inactive based on the
  /// input boolean.
  /// @param enabled - `true` if the menu is enabled.
  void setEnabled(bool enabled) noexcept;

  /// @brief - Return the background color for this menu.
  /// @return - the background color.
  olc::Pixel getBackgroundColor() const noexcept;

  /// @brief- Interface method allowing to render a menu in a parent application.
  /// This is used to offload some of the rendering code from the main app and
  /// hide the internal complexity of the menu.
  /// Note: we draw on the active layer so it has to be configured before calling
  /// this method
  /// *@param pge - the rendering engine to display the menu.
  void render(olc::PixelGameEngine *pge) const;

  /// @brief - Used to process the user input defined in the argument and update
  /// the internal state of this menu if needed.
  /// @param c - the controls and user input for this frame.
  /// @param actions - the list of actions produced by the menu while processing the
  /// events.
  /// @return - the description of what happened when the inputs has been processed.
  menu::InputHandle processUserInput(const controls::State &c, std::vector<ActionShPtr> &actions);

  /// @brief - Adds the input menu as a child of this one.
  /// @param child - the child menu to register.
  void addMenu(MenuShPtr child);

  /// @brief - Retrieve the size for this menu.
  /// @return - the size of this menu in pixels.
  olc::vf2d getSize() const noexcept;

  /// @brief - Returns the text associated to this menu or the empty string if none
  /// is attached.
  /// @return - the text content of this menu.
  std::string getText() const noexcept;

  /// @brief - Replace the existing background content with the new one.
  /// @param bg - the new background description.
  void setBackground(const menu::BackgroundDesc &bg);

  /// @brief - Replace the existing content with the new one.
  /// @param mcd - the new content description for this menu.
  void setContent(const menu::MenuContentDesc &mcd);

  /// @brief - Replace the existing text with the new one. It will keep every
  /// other foreground properties in a similar state.
  void setText(const std::string &text);

  /// @brief - Assigns a new action attached to this menu.
  /// @param action - the action attached to the menu.
  void setAction(menu::RegisterAction action);

  /// @brief - Used to define a new simple action where the process would be to
  /// push the input callback to the provided vector of actions.
  /// @param process - the callback to be pushed to the list of action whenever
  /// the callback linked to this menu is triggered.
  void setSimpleAction(action::Process process);

  protected:
  /// @brief - Interface method allowing inheriting classes to perform their own
  /// drawing routines on top of the base representation of the menu.
  /// This default implementation does nothing.
  /// @param pge - the rendering engine to display the menu.
  virtual void renderSelf(olc::PixelGameEngine *pge) const;

  /// @brief - Interface method allowing inheriting classes to perform the creation
  /// of their own actions whenver a click is detected on this menu.
  /// For now this method is only triggered when a click with the left mouse button
  /// is detected. The default implementation does nothing.
  /// @param actions - output vector to register actions if needed.
  virtual void onClick(std::vector<ActionShPtr> &actions) const;

  /// @brief - Interface method called right before this menu is changed to selected:
  /// this gives the chance to inheriting classes to react to a user event.
  /// The default implementation returns `true`.
  /// @return - `true` if the event should be processed.
  virtual bool onHighlight() const;

  /// @brief - Used to obtain the absolute position of the menu within the app,
  /// considering the position of the parent (if defined).
  /// @return - the absolute position of this menu.
  olc::vi2d absolutePosition() const noexcept;

  /// @brief - Allow inheriting classes to access the background description for
  /// this menu as a copy.
  /// @return - a copy of the background description.
  menu::BackgroundDesc bg() const noexcept;

  /// @brief - Allow inheriting classes to access the foreground description for
  /// this menu as a copy.
  /// @return - a copy of the foreground description.
  menu::MenuContentDesc fg() const noexcept;

  private:
  /// @brief - Used to perform the loaded of the foreground tile used by this
  /// menu (i.e. the content tile).
  void loadFGTile();

  /// @brief - Clear any loaded resource for this menu. Used when the visual
  /// appearance needs to be adjusted.
  void clear();

  /// @brief - Clear any loaded resource for the content of this menu. Used when
  /// the visual appearance needs to be adjusted.
  void clearContent();

  /// @brief - Used to adapt the size of the children menus as required by the
  /// actual size of this menu.
  void updateChildren();

  private:
  /// @brief - Convenience structure describing the current state for this
  /// menu, such as visibility and highlight.
  struct State
  {
    // Whether this menu is currently visible.
    bool visible;

    // Whether this menu is currently enabled.
    bool enabled;

    // Whether this menu is clickable: this triggeres whether a
    // hovering over action is associated with a change in color.
    bool clickable;

    // Whether this menu is selectable. It is used to define
    // whether the selected color should be applied in case
    // of a click within the menu.
    bool selectable;

    // Whether or not this menu is currently highlighted.
    bool highlighted;

    // Whether or not this menu is currently selected.
    bool selected;
  };

  /// @brief - Describe the current state for this menu. It is used as a way to
  /// regroup all information needed to repaint this component.
  State m_state;

  /// @brief - The position of the menu in screen coordinates. It corresponds to
  /// the position of the top left corner.
  olc::vf2d m_pos;

  /// @brief - The size of the menu in pixels.
  olc::vi2d m_size;

  /// @brief - The background associated to this menu. Can either represent a
  /// uniform color or a tiled image.
  menu::BackgroundDesc m_bg;

  /// @brief - Describe the properties associated to the content of this menu.
  menu::MenuContentDesc m_fg;

  /// @brief - Hold the sprite used as an icon for this menu. It might be `null`
  /// in case none is used in the menu's content.
  olc::Decal *m_fgSprite;

  /// @brief - The layout for this menu. Allow to define how the children will
  /// be displayed in this menu.
  menu::Layout m_layout;

  /// @brief - The parent menu for this element. Can be `null` in case the menu
  /// is the root of the subsystem.
  Menu *m_parent;

  /// @brief - The list of children menu for this element. Children may or may
  /// not be active and are always repainted on the parent.
  std::vector<MenuShPtr> m_children;

  /// @brief - The callback to trigger whenver this menu is clicked upon.
  menu::RegisterAction m_callback;
};

} // namespace pge

#include "Menu.hxx"
