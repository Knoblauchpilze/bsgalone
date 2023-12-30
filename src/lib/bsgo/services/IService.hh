
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class Coordinator;

class IService : public utils::CoreObject
{
  public:
  IService(const std::string &name);
  ~IService() override = default;

  virtual bool isReady() const noexcept = 0;
};

using IServicePtr = std::unique_ptr<IService>;

} // namespace bsgo
