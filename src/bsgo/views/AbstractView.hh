
#pragma once

#include "IView.hh"
#include <core_utils/CoreObject.hh>
#include <string>

namespace bsgo {

class AbstractView : public utils::CoreObject, public IView
{
  public:
  AbstractView(const std::string &name);
  ~AbstractView() override = default;

  bool isReady() const noexcept override;
};

} // namespace bsgo
