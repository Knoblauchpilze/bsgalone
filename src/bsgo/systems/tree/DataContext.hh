
#pragma once

#include <optional>

namespace bsgo {

class DataContext
{
  public:
  DataContext();
  DataContext(const int targetIndex);
  ~DataContext() = default;

  auto tryGetTargetIndex() const -> std::optional<int>;
  void setTargetIndex(const int targetIndex);
  void clearTargetIndex();

  bool changed() const;
  void markAsSynced();

  private:
  bool m_changed{false};

  std::optional<int> m_targetIndex{};
};

} // namespace bsgo
