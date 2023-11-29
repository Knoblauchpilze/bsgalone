
#pragma once

#include "IView.hh"
#include <memory>

namespace bsgo {

class LoginView : public IView
{
  public:
  LoginView(const CoordinatorShPtr &coordinator, const Repositories &repositories);
  ~LoginView() override = default;

  auto tryLogin(const std::string &name, const std::string &password) const -> std::optional<Uuid>;
};

using LoginViewShPtr = std::shared_ptr<LoginView>;

} // namespace bsgo