#ifndef __MY_EXCEPTION_
#define __MY_EXCEPTION_

#include <exception>
#include <cstring>
#include <iostream>

#define ERROR_MSG_SIZE 512

using namespace std;

inline void cstrcat(char* dest, const char* src, size_t len) {
  auto destLen = strlen(dest);
  strncat(dest, src, len > (destLen + 1) ? (len - destLen - 1) : 0);
}

class MyException : public exception {
public:
  MyException(const char * func);

  virtual const char * what() const noexcept override { return msg; }
protected:
  char msg[ERROR_MSG_SIZE] = {0};
};

 
namespace nsMyException {
  inline void pError(MyException& e) {
    cerr << "\033[0;31m" << e.what() << "\033[0m" << endl;
  }
}

#endif


