
#pragma once

#include <optional>
#include <string>
#include <uuid.h>

namespace bsgalone::core {

class Uuid
{
  public:
  Uuid()  = default;
  ~Uuid() = default;

  /// @brief - Returns a string representing the identifier. This can be used for
  /// display purposes.
  /// @return - a string representing the identifier.
  auto str() const -> std::string;

  /// @brief - Converts this identifier into a representation compatible to be
  /// written to the database. This is implemented by calling the `str` method.
  /// @return - a string representing the identifier
  auto toDbId() const -> std::string;

  /// @brief - Generates a new random identifier.
  /// @return - the new identifier
  static auto random() -> Uuid;

  /// @brief - Attempts to parse an identifier fetched from the database into
  /// a valid uuid. In case this fails, an empty optional is returned.
  /// @param dbId - the string describing an identifier from the database
  /// @return - when available the uuid parsed from the input string
  static auto fromDbId(const std::string_view dbId) -> std::optional<Uuid>;

  /// @brief - Tries to read a uuid from the input stream. If it
  /// succeeds, a valid message will be returned, otherwise an empty
  /// optional.
  /// The stream will **not be reset** in case the reading fails.
  /// @param in - the stream to read from
  /// @return - the deserialized message or an empty optional if reading
  /// the message's data fails
  static auto readFromStream(std::istream &in) -> std::optional<Uuid>;

  bool operator==(const Uuid &rhs) const;

  private:
  uuids::uuid m_uuid{};

  Uuid(uuids::uuid uuid);

  friend auto operator<<(std::ostream &out, const Uuid &uuid) -> std::ostream &;
  friend std::hash<Uuid>;
};

} // namespace bsgalone::core

#include "Uuid.hxx"
