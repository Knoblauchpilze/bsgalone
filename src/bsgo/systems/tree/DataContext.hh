
#pragma once

#include <optional>

namespace bsgo {

class DataContext
{
  public:
  DataContext();
  ~DataContext() = default;

  auto tryGetTargetIndex() const -> std::optional<int>;
  void setTargetIndex(const int targetIndex);

  bool changed() const;
  void markAsSynced();

  private:
  bool m_changed{false};

  std::optional<int> m_targetIndex{};
};

} // namespace bsgo
