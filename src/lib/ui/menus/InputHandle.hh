
#pragma once

namespace pge::menu {

/// @brief - Convenience struct defining what can happen while processing
/// inputs. It is mostly used internally to detect update of children but
/// can also be interpreted by external callers if needed.
struct InputHandle
{
  bool relevant{false};
  bool selected{false};
};

} // namespace pge::menu
