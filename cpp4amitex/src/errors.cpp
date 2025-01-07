#include "amitex/errors.hpp"

namespace amitex {

InputError::InputError(std::string_view msg) : msg{} {
  this->msg = "AMITEX Input Error: " + std::string{msg};
}
const char* InputError::what() const noexcept { return msg.c_str(); }

AmitexError::AmitexError(std::string_view msg, int code) : msg{}, code{code} {
  this->msg = "AMITEX (code " + std::to_string(code) + "): " + std::string{msg};
}
AmitexError::AmitexError(std::string_view msg) : msg{}, code{0} {
  this->msg = "AMITEX: " + std::string{msg};
}
const char* AmitexError::what() const noexcept { return msg.c_str(); }

void driverInputHandler(char* message, int code) { throw AmitexError{std::string{message}, code}; }

}  // namespace amitex
