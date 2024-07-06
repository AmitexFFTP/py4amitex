#include "amitex/errors.hpp"

namespace amitex {

InputError::InputError(std::string_view msg) {
  this->msg = "AMITEX Input Error: " + std::string{msg};
}
const char* InputError::what() const noexcept { return msg.c_str(); }

AmitexError::AmitexError(std::string_view msg, int code) {
  this->code = code;
  this->msg = "AMITEX (code " + std::to_string(code) + "): " + std::string{msg};
}
AmitexError::AmitexError(std::string_view msg) {
  this->msg = "AMITEX: " + std::string{msg};
  this->code = 0;
}
const char* AmitexError::what() const noexcept { return msg.c_str(); }

void driverInputHandler(char* message, int code) { throw AmitexError{std::string{message}, code}; }

}  // namespace amitex
