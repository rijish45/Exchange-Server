#include "connectionPool.hpp"

ConnectionPool* ConnectionPool::pool = nullptr;

ConnectionPool::ConnectionPool(string _dbname, string _username, string _password, int _poolSize):
                               dbname(_dbname), username(_username), password(_password), poolSize(_poolSize) {
  // "dbname=exchange_server user=postgres password = password"
  
  connectionUrl = "dbname=" + dbname + " user=" + username + " password=" + password + " host=db";
  initConnectPool(poolSize/2);
  // initializeDB();
}

ConnectionPool::~ConnectionPool() {
  destoryPool();
}

int ConnectionPool::getPoolSize() {  
  lock_guard<mutex> locker(lock);
  return conList.size();
}

void ConnectionPool::addConn(int size) {
  for (int i = 0; i < size; ++i) {
    connection * conn = new connection(connectionUrl);
    shared_ptr<connection> sp(conn, [](connection *conn) {
	delete conn;
      });
    conList.push_back((move(sp)));
  }
}

void ConnectionPool::initConnectPool(int initialSize) {
  lock_guard<mutex> locker(lock);
  addConn(initialSize);
}

void ConnectionPool::destoryOneConn() {
  // 智能指针加std::move转移一个连接的“所有权”，当出作用域时，自动调用关闭connect
  shared_ptr<connection> &&sp = move(conList.front());
  sp->disconnect();
  --poolSize;
}

void ConnectionPool::destoryPool() {
  for (auto &conn : conList) {
    shared_ptr<connection> &&sp = move(conn);
    sp->disconnect();
  }
}

ConnectionPool* ConnectionPool::getInstance() {
  if (pool == nullptr) {
    pool = new ConnectionPool("exchange_server", "postgres", "password", 20);
  }
  return pool;
}

shared_ptr<connection> ConnectionPool::getConnect() {
  lock_guard<mutex> locker(lock);
  shared_ptr<connection> sp = conList.front();
  conList.pop_front();
  return sp;
}


void ConnectionPool::retConnect(shared_ptr<connection> &ret) {
  lock_guard<mutex> locker(lock);
  conList.push_back(ret);
}
