
#pragma once

#include <core_utils/CoreObject.hh>

namespace bsgo {

class IRepository : public utils::CoreObject
{
  public:
  IRepository(const std::string &name);
  ~IRepository() override = default;

  /// @brief - To reimplement to prepare the queries used by this repository.
  /// This is useful to make the subsequent executions of the queries faster.
  /// See: https://libpqxx.readthedocs.io/en/6.4/a01480.html
  virtual void initialize();
};

} // namespace bsgo
