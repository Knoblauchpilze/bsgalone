
#pragma once

#include <concepts>
#include <string>

namespace messaging {

// https://stackoverflow.com/questions/60524480/how-to-force-a-template-parameter-to-be-an-enum-or-enum-class
template<typename Type>
concept EventTypeLike = std::is_enum_v<Type> && requires(Type e) {
  {
    str(e)
  } -> std::convertible_to<std::string>;
};

} // namespace messaging
