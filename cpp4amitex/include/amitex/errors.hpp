#ifndef _AMITEX_ERRORS_HEADER_
#define _AMITEX_ERRORS_HEADER_

#include <exception>
#include <string>
#include <string_view>

//! \file errors.hpp
//! Define Exceptions and error handling utilities

namespace amitex {

//! Thrown by input parametrizing
class InputError : public std::exception {
 public:
  InputError(std::string_view msg);
  ~InputError() override = default;
  InputError& operator=(const InputError&) noexcept = default;
  const char* what() const noexcept override;

 private:
  std::string msg;
};

//! Thrown by execution of AMITEX-FFTP
//! \warning
//! Trying to continue a simulation (e.g. a new timestep) afterwards is undefined
class AmitexError : public std::exception {
 public:
  AmitexError(std::string_view msg);
  AmitexError(std::string_view msg, int code);
  ~AmitexError() override = default;
  AmitexError& operator=(const AmitexError&) noexcept = default;
  const char* what() const noexcept override;

 private:
  std::string msg;
  int code;
};

//! Connected to AMITEX external driving API so that AmitexError are thrown
//! \throw AmitexError
[[noreturn]] void driverInputHandler(char* message, int code);

}  // namespace amitex

#endif  // _AMITEX_LOADING_HEADER_
