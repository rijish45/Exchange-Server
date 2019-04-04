#ifndef __TRANSACTION
#define __TRANSACTION

#include <vector>
#include <iostream>
#include "pugixml.hpp"
 
using namespace std;
using namespace pugi;

class Transaction {
public:
  Transaction() = delete;
  Transaction(string xml);
  Transaction(Transaction && rhs) = default;
  Transaction & operator=(Transaction && rhs) = default;
  Transaction(const Transaction & rhs) = default;
  Transaction & operator=(const Transaction & rhs) = default;
  class Order {
    string sym;
    string amount; // int?
    string limit; // int?
  public:
    Order() = delete;
    Order(string _sym, string _amount, string _limit);
    string getSym() const { return sym; }
    string getAmount() const { return amount; }
    string getLimit() const { return limit; }    
  };
  class Query {
    string transId; // int ?
  public:
    Query() = delete;
    Query(string _transId);
    string getTransId() const { return transId; }
  };
  class Cancel { 
    string transId; // int?
  public:
    Cancel() = delete;
    Cancel(string _transId);
    string getTransId() const { return transId; }
  };
  const string getAccountId() const { return accountId; }
  const vector<Order>  getOrders() const { return orders; }
  const vector<Query> getQueries() const { return queries; }
  const vector<Cancel> getCancels() const { return cancels; }
  bool hasOrders() { return !orders.empty(); }
  bool hasQueries() { return !queries.empty(); }
  bool hasCancels() { return !cancels.empty(); }  
  ~Transaction();
private:
  string xml;
  string accountId; // int?
  vector<Order> orders;
  vector<Query> queries;
  vector<Cancel> cancels;
  void parseTrans();
  void parseHelper(xml_node & node);
};


#endif
