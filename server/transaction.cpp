#include "transaction.hpp"


void Transaction::parseTrans() {
  // let the parse check if xml is valid
  xml_document doc;
  xml_parse_result result = doc.load_string(xml.c_str(), sizeof(xml.c_str()));
  if (!result) {
    cerr << "Parse error: " << result.description() << ", character pos = " << result.offset << endl;
    // throw exception
    throw 12345;
  }
  // manully check if xml is valid
  xml_node root = doc.document_element();
  if (strcmp(root.name(), "transactions") == 0) {
    parseHelper(root);
  }
}
  
void Transaction::parseHelper(xml_node & node) {
  
  accountId = node.attribute("id").value();
  // cout << "AccountID "<< accountId << endl;
  for (xml_node_iterator it  = node.begin(); it != node.end(); ++it) {
    
    string name = string(it->name());
    if (name == "order" && it->attribute("sym") && it->attribute("amount") && it->attribute("limit")) {
      string sym = it->attribute("sym").value();
      string amount = it->attribute("amount").value();
      string limit = it->attribute("limit").value();
      // cout << "sym " << sym << endl;
      // cout << "amount " << amount << endl;
      // cout << "limit " << limit << endl;
      orders.push_back(Order(sym, amount, limit));
    }

    if (name == "query" && it->attribute("id")) {
      string transId = it->attribute("id").value();
      // cout << "transId in query " << transId << endl;
      queries.push_back(Query(transId));
    }

    if (name == "cancel" && it->attribute("id")) {
      string transId = it->attribute("id").value();
      // cout << "transId in cancel " << transId << endl;
      cancels.push_back(Cancel(transId));
    }
  }
}

Transaction::Transaction(string _xml) : xml(_xml) {
  parseTrans();
}

Transaction::~Transaction() {}


Transaction::Order::Order(string _sym, string _amount, string _limit) : sym(_sym), amount(_amount), limit(_limit) {}

Transaction::Query::Query(string _transId) : transId(_transId) {}

Transaction::Cancel::Cancel(string _transId) : transId(_transId) {}
									
