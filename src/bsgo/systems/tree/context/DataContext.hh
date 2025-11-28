
#pragma once

#include "ContextKey.hh"
#include "DataKey.hh"
#include <optional>
#include <unordered_map>

namespace bsgo {

class DataContext
{
  public:
  DataContext()  = default;
  ~DataContext() = default;

  template<typename Value>
  void setKey(const ContextKey key, const Value &value);

  template<typename Value>
  auto tryGetKey(const ContextKey key) const -> std::optional<Value>;

  void clear(const ContextKey key);

  bool changed() const;
  void markAsSynced();

  private:
  using KeyMap = std::unordered_map<ContextKey, DataKey>;

  /// @brief - Allows to track changes to the keys themselves. For example when
  /// a key is removed from the `m_keys` map, there's no way the map can be used
  /// to keep track of it. This is the purpose of this boolean.
  bool m_changed{false};
  KeyMap m_keys{};
};

} // namespace bsgo

#include "DataContext.hxx"
