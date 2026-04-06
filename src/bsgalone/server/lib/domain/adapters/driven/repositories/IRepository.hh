
#pragma once

namespace bsgalone::server {

class IRepository
{
  public:
  IRepository()          = default;
  virtual ~IRepository() = default;

  /// @brief - To reimplement to prepare the queries used by this repository.
  /// This is useful to make the subsequent executions of the queries faster.
  /// See: https://libpqxx.readthedocs.io/en/6.4/a01480.html
  virtual void initialize() = 0;
};

} // namespace bsgalone::server
