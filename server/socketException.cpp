#include "socketException.hpp"
#include <iostream>

using namespace nsConnectionError;

ConnectionError::ConnectionError(ErrorNum errorNum, const char* func) :
  MyException(func),
  errorNum(errorNum) {
  cstrcat(this->msg, ERROR_MSG[static_cast<int>(this->errorNum)], ERROR_MSG_SIZE);
}

ConnectionError::ConnectionError(ErrorNum errorNum, const char* port, const char* func) :
  MyException(func),
  errorNum(errorNum) {
  cstrcat(this->msg, ERROR_MSG[static_cast<int>(this->errorNum)], ERROR_MSG_SIZE);
  cstrcat(this->msg, "\nport: ", ERROR_MSG_SIZE);
  cstrcat(this->msg, port, ERROR_MSG_SIZE);
}

ConnectionError::ConnectionError(ErrorNum errorNum, const char* host, const char* port, const char* func) :
  MyException(func),
  errorNum(errorNum) {
  cstrcat(this->msg, ERROR_MSG[static_cast<int>(this->errorNum)], ERROR_MSG_SIZE);
  cstrcat(this->msg, "\nhost: ", ERROR_MSG_SIZE);
  cstrcat(this->msg, host, ERROR_MSG_SIZE);
  cstrcat(this->msg, "\nport: ", ERROR_MSG_SIZE);
  cstrcat(this->msg, port, ERROR_MSG_SIZE);
}
