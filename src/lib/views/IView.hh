
#pragma once

#include <core_utils/CoreObject.hh>
#include <memory>

namespace bsgo {

class IView : public utils::CoreObject
{
  public:
  IView(const std::string &name);
  virtual ~IView() = default;
};

using IViewShPtr = std::shared_ptr<IView>;

} // namespace bsgo
