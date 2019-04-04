#ifndef __SOCKET_EXCEPTION_H__
#define __SOCKET_EXCEPTION_H__

#include "myException.hpp"
#include <iterator>
 

namespace nsConnectionError {
  const char* const ERROR_MSG[] = {
    "Error: cannot get address info",
    "Error: cannot create socket",
    "Error: cannot bind socket",
    "Error: cannot listen on socket",
    "Error: cannot connect",
  };

  enum class ErrorNum {
    AddressTranslationFailure,
      SocketCreationFailure,
      BindingFailure,
      ListeningFailure,
      ConnectionFailure,
      };

  // static_assert(std::size(ERROR_MSG) == static_cast<size_t>(ErrorNum::Count), "In nsConnectionError: Size of ERROR_MSG and ErrorNum mismatches");
}

class ConnectionError : public MyException {
public:
  ConnectionError(nsConnectionError::ErrorNum errorNum, const char* func);
  ConnectionError(nsConnectionError::ErrorNum errorNum, const char* port, const char* func);
  ConnectionError(nsConnectionError::ErrorNum errorNum, const char* host, const char* port, const char* func);

  nsConnectionError::ErrorNum getErrorNum() const noexcept { return this->errorNum; }
private:
  nsConnectionError::ErrorNum errorNum;
};

class AcceptError : public MyException {
public:
  AcceptError(const char* port, const char* func) : MyException(func) {
    cstrcat(this->msg, "Acception Fails on port: ", ERROR_MSG_SIZE);
    cstrcat(this->msg, port, ERROR_MSG_SIZE);
  }
};


class MsgTransmitError : public MyException {
public:
  MsgTransmitError(const char* func) : MyException(func) {}
};

#endif
