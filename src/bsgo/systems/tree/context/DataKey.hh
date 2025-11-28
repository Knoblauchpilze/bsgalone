
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
  static auto fromValue(const Value &value) -> DataKey;

  private:
  std::variant<Uuid> m_key;
};

} // namespace bsgo

#include "DataKey.hxx"
