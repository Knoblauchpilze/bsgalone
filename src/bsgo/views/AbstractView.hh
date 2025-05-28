
#pragma once

#include "CoreObject.hh"
#include "IView.hh"
#include <string>

namespace bsgo {

class AbstractView : public core::CoreObject, public IView
{
  public:
  AbstractView(const std::string &name);
  ~AbstractView() override = default;

  bool isReady() const noexcept override;
};

} // namespace bsgo
