#include "myException.hpp"

MyException::MyException(const char* func) {
  strncpy(this->msg, "In function: ", ERROR_MSG_SIZE);
  cstrcat(this->msg, func, ERROR_MSG_SIZE);
  cstrcat(this->msg, ": ", ERROR_MSG_SIZE);
}
