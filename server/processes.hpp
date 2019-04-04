#ifndef __PROCESS__
#define __PROCESS__

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <vector>
#include <pqxx/pqxx>
#include <string>
#include <fstream>
#include <mutex>
#include <atomic>
#include "transaction.hpp"
#include "connectionPool.hpp"
#include "pugixml.hpp"
#include "socket.hpp"

using namespace std;
using namespace pqxx;
using namespace pugi;
using namespace std::chrono;

// connection * C;
// mutex mutex_t;
// extern ConnectionPool * C;

void initializeDB(ConnectionPool * pool);

void drop_all_tables(connection * C);

void create_account_table(connection * C);

void match_order(string account_id, string order_id, string symbol, string amount, string price_limit, string & response , connection * C);

void add_order(string account_id, string order_id, string symbol, string amount, string price_limit, string & response , connection * C);

void match_order(string account_id, string order_id, string symbol, string amount, string price_limit, string & response , connection * C);

void handle_share_creation(string & response, string symbol, string id, string amount, connection * C);

void handle_account_creation(string balance, string id, string & response, connection * C);

void create_shares_table(connection * C);

void executed_shares_table(connection * C);

void create_open_orders(connection * C);

void create_closed_orders(connection * C);

int parse_xml(string & xml);

void parse_create_xml(xml_node & node, string & response, connection * C);
 
void handle_request_all(string xml, string response, ConnectionPool * pool, Socket && clientSocket);

void cancel_response(string order_id, string account_id, string & response, connection * C);

void cancel_order(string order_id, string account_id, string & response, connection * C);

#endif


