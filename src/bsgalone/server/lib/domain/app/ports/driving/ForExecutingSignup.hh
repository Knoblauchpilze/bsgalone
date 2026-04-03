
#pragma once

#include "SignupData.hh"
#include <memory>

namespace bsgalone::core {

class ForExecutingSignup
{
  public:
  ForExecutingSignup()          = default;
  virtual ~ForExecutingSignup() = default;

  virtual void performSignup(const SignupData &data) = 0;
};

using ForExecutingSignupPtr = std::unique_ptr<ForExecutingSignup>;

} // namespace bsgalone::core
