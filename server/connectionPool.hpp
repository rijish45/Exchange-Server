#ifndef __CONNECTION_POOL
#define __CONNECTION_POOL

#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <functional>
#include <cstdio>
#include <cstdlib>
#include <assert.h>
#include <list>
#include <mutex>
// #include "processes.hpp"

using namespace std;
using namespace pqxx;
using delFunc = function<void(connection*)>;

class ConnectionPool {
public:
  
  static ConnectionPool* getInstance();
  
  auto getConnect()->shared_ptr<connection>;
  
  auto retConnect(shared_ptr<connection> &ret)->void;
  
  ~ConnectionPool();

private:
  ConnectionPool(string _dbname, string _username, string _password, int _poolSize);

  // void initializeDB();
  // initialize the pool
  auto initConnectPool(int initialSize)->void;
  // destory pool
  auto destoryPool()->void;
  //destory one connection
  auto destoryOneConn()->void;
  // add conn
  auto addConn(int size)->void;

public:
  //get size
  auto getPoolSize()->int;

private:
  string dbname;
  string username;
  string password;
  int poolSize;
  string connectionUrl;
  list<shared_ptr<connection>> conList;
  static ConnectionPool * pool;
  mutex lock;
};


#endif
