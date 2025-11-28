
#pragma once

#include "Uuid.hh"
#include <variant>

namespace bsgo {

class DataKey
{
  public:
  DataKey()  = default;
  ~DataKey() = default;

  template<typename Value>
  auto as() const -> Value;

  template<typename Value>
  void set(const Value &value);

  bool changed() const;
  void markAsSynced();

  template<typename Value>
  static auto fromValue(const Value &value) -> DataKey;

  private:
  bool m_changed{false};
  std::variant<Uuid> m_key;
};

} // namespace bsgo

#include "DataKey.hxx"
