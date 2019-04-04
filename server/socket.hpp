#ifndef __SOCKET__
#define __SOCKET__

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <netdb.h>
#include <utility>
#include "socketException.hpp"

#define BUF_SIZE 812 
#define CH_SIZE 1

using namespace std;

class Socket {
protected:
  int socket_fd;
private:
  string recvMessage(size_t bytes); // reveive all message (unnecessary to be virtual)
  int parseBytes(string & integerLine);
public:
  Socket();
  Socket(int __socket_fd);
  Socket(const Socket & rhs) = delete; // copy constructor
  Socket & operator=(const Socket & rhs) = delete; // assignment operator
  Socket(Socket && rhs) noexcept;
  Socket& operator=(Socket && rhs) noexcept;
  bool isValid();
  string recvXml();
  size_t sendMessage(string message); // send all message
  virtual ~Socket();
};

class Listener : public Socket {
  // socket_fd
public:
  Listener();
  Listener(const Listener & rhs) = delete; // copy constructor
  Listener & operator=(const Listener & rhs) = delete; // assignment operator  
  Listener(Listener && rhs) noexcept;
  Listener & operator=(Listener && rhs) noexcept;
  Socket Accept();
  ~Listener();
};

class Connector : public Socket {
  const char * port;
  const char * host;
public:
  Connector() = delete;
  Connector(const char * port, const char * host);
  Connector(const Connector & rhs) = delete; // copy constructor
  Connector & operator=(const Connector & rhs) = delete; // assignment operator
  Connector (Connector && rhs) noexcept;
  Connector & operator=(Connector && rhs) noexcept;
  ~Connector();
};


#endif
