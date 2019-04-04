#include <iostream>
#include <thread> 
#include "socket.hpp"
#include "transaction.hpp"
#include "processes.hpp"
#include "connectionPool.hpp"


using namespace std;
 
int main() {
  ConnectionPool * dbPool = ConnectionPool::getInstance(); // may throw exception
  unique_ptr<Listener> serverSocket;
  try {
    serverSocket.reset(new Listener());
  } catch (...) {
     // cerr << e.what() << endl;
     return EXIT_FAILURE;
  }
  initializeDB(dbPool);
  while (true) {
    Socket clientSocket;
    try {
      clientSocket = serverSocket->Accept();
    } catch (...) {
      // cerr << e.what() << endl;
      continue;
    }
    string xml = clientSocket.recvXml();
    // cout << xml << endl;
    string response = "";  
    thread th(handle_request_all, xml, response, dbPool, move(clientSocket));
    th.detach();      
  }  
}
