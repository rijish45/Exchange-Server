#include "processes.hpp"
#include <atomic>
//ConnectionPool * C = ConnectionPool::getInstance();
mutex mutex_t;
int global_order_id (0);

//void match_order(string account_id, string order_id, string symbol, string amount, string price_limit, string & response , connection * C);

  void initializeDB(ConnectionPool * pool) {
  shared_ptr<connection> ptr = pool->getConnect();
  connection * C = ptr.get();
  try {
     drop_all_tables(C);
     create_account_table(C);
     create_shares_table(C);
     create_open_orders(C);
     executed_shares_table(C);
     create_closed_orders(C);
  } catch (exception & e) {
    cerr << e.what() << endl;
    pool->retConnect(ptr);
    exit(EXIT_FAILURE);
  }
  }


void cancel_response(string order_id, string account_id, string & response, connection * C){

  work W (*C);
  result R;
  result::const_iterator res;
  string sql;
 
  try {
  
     sql = "SELECT AMOUNT, TIME FROM CLOSED_ORDERS WHERE ACCOUNT_ID = " + W.quote(account_id) + " AND ORDER_ID = " + W.quote(order_id) + " ;";
     R = W.exec(sql);
     res = R.begin();
    if(res != R.end()){
      float canceled_amount = res[0].as<float>();
      long long cancel_time = res[1].as<long long>();

      response += " <canceled shares= ";
      response += to_string(canceled_amount);
      response += " time = ";
      response += to_string(cancel_time);
      response += "/>\n";
    }

    sql =  "SELECT AMOUNT, PRICE, TIME FROM EXECUTED_SHARES WHERE ID = " + W.quote(order_id) + " ;";
    R = W.exec(sql);
    res = R.begin();
    if(res == R.end()){

      response += "</canceled>\n";
      return; 
    }
    else{

      float amount_executed = res[0].as<float>();
      float price_execution = res[1].as<float>();
      long long time_execution = res[2].as<long long>();

      response += " <executed shares= ";
      response += to_string(amount_executed);
      response += " price = ";
      response += to_string(price_execution);
      response += " time = ";
      response += to_string(time_execution);
      response += " />\n";
      response += "</canceled>\n";
      return;

    }


  }

  catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return ;
  }


}



void cancel_order(string order_id, string account_id, string & response, connection * C){

  work W (*C);
  result R;
  result::const_iterator res;
  string sql;
 

  float amount;
  float price_limit;
  string symbol;
  
  try{

    //Update Open Orders
    sql = "SELECT AMOUNT, SYMBOL, PRICE_LIMIT FROM OPEN_ORDERS WHERE ACCOUNT_ID = " + W.quote(account_id) + " AND ORDER_ID = " + W.quote(order_id) + " ;";
    R = W.exec(sql);
    res = R.begin();
    if(res == R.end()){

      // lock_guard<mutex> lock(mutex_t);
      response += "  <error id=\"" + order_id + "\"> Order does not exist. </error>\n";
      return;

    }

    amount = res[0].as<float>();
    symbol = res[1].as<string>();
    price_limit = res[2].as<float>();

    if (amount == 0) {

      // lock_guard<mutex> lock(mutex_t);
      response += "  <error id=\"" + order_id + "\"> Order has been executed completely. Nothing to cancel. </error>\n";
      return;
    }

    //Sell order 
    if( amount < 0) {

      //First we need to update open orders
      sql = "UPDATE OPEN_ORDERS SET AMOUNT = 0 WHERE ACCOUNT_ID = " +  W.quote(account_id) + " AND ORDER_ID = " + W.quote(order_id) + " ; ";
      W.exec(sql);

      sql = "SELECT AMOUNT FROM SHARES WHERE ACCOUNT_ID = " + W.quote(account_id) + " AND SYMBOL = " + W.quote(symbol) + " ;";
      R = W.exec(sql);

      res = R.begin();
      float existing_amount = res[0].as<float>();
      float new_amount = existing_amount + abs(amount);
      sql = "UPDATE SHARES SET AMOUNT = " + W.quote(new_amount) + " WHERE ACCOUNT_ID = " +  W.quote(account_id) + " AND SYMBOLS = " + W.quote(symbol) + " ; ";

    }

    //buy order
    else if (amount > 0){

      sql = "UPDATE OPEN_ORDERS SET AMOUNT = 0 WHERE ACCOUNT_ID = " +  W.quote(account_id) + " AND ORDER_ID = " + W.quote(order_id) + " ; ";
      W.exec(sql);

      sql = "SELECT BALANCE FROM ACCOUNT WHERE ACCOUNT_ID = " + W.quote(account_id) +  " ;";
      R = W.exec(sql);
      res = R.begin();
      float previous_balance = res[0].as<float>();
      float new_balance = previous_balance + (price_limit * amount);

      sql = "UPDATE ACCOUNT SET BALANCE = " + W.quote(new_balance) + " WHERE ACCOUNT_ID = " + W.quote(account_id) +  " ;";
      W.exec(sql);
      
    }
    

    //Update Closed Orders
    time_t result = time(nullptr);
    sql = "INSERT INTO CLOSED_ORDERS (ACCOUNT_ID, ORDER_ID, SYMBOL, AMOUNT, PRICE_LIMIT, TIME) VALUES (";
    sql += W.quote(account_id) + ", ";
    sql += W.quote(order_id) + ", ";
    sql += W.quote(symbol) + ", ";
    sql += W.quote(amount) + ", ";
    sql += W.quote(price_limit) + ", ";
    sql += W.quote(result)+ ");";

    W.exec(sql);
    W.commit();
    cancel_response(order_id, account_id, response,  C);

  }
  
  catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return ;
  }

}
  
  

void query_order(string query_id, string account_id, string & response, connection * C){


  work W (*C);
  result R;
  result::const_iterator res;
  string sql;
  


    sql = "SELECT AMOUNT FROM OPEN_ORDERS WHERE ACCOUNT_ID = " + W.quote(account_id) + " AND ORDER_ID = " + W.quote(query_id) + " ;";
    R = W.exec(sql);
    res = R.begin();
    if(res != R.end()){

      float open_shares = res[0].as<float>();
      // cout << open_shares << endl;
      response += " <open shares = ";
      response += to_string(open_shares);
      response += " />\n";

    }

    sql = "SELECT AMOUNT, TIME FROM CLOSED_ORDERS WHERE ACCOUNT_ID = " + W.quote(account_id) + " AND ORDER_ID = " + W.quote(query_id) + " ;";
    R = W.exec(sql);
    res = R.begin();
    if(res != R.end()){
      float canceled_amount = res[0].as<float>();
      long long cancel_time = res[1].as<long long>();

      response += " <canceled shares= ";
      response += to_string(canceled_amount);
      response += " time = ";
      response += to_string(cancel_time);
      response += "/>\n";
    }

    sql =  "SELECT AMOUNT, PRICE, TIME FROM EXECUTED_SHARES WHERE ID = " + W.quote(query_id) + " ;";
    R = W.exec(sql);
    res = R.begin();
    if(res == R.end()){

      response += "</status>\n";
      return; 
    }
    else{

      float amount_executed = res[0].as<float>();
      float price_execution = res[1].as<float>();
      long long time_execution = res[2].as<long long>();

      response += " <executed shares= ";
      response += to_string(amount_executed);
      response += " price = ";
      response += to_string(price_execution);
      response += " time = ";
      response += to_string(time_execution);
      response += " />\n";
      response += "</status>\n";
      return;

    }

}



void add_order(string account_id, string order_id, string symbol, string amount, string price_limit, string & response , connection * C){


  string sql;
  result R;
  work W(*C);
  result::const_iterator res;


  try{

    //check if price is right
    float price = stof(price_limit);
    if(price <= 0){

      response += " <error sym=\"" + symbol + "\" amount=\"" + to_string(abs(stof(amount))) + "\" limit=\"" + price_limit + "\"> Invalid Price Limit. </error>\n";
      return;

    }

    //check if the account_id is correct
    long long acc_id = stoll(account_id);
    if(acc_id <= 0){
      response += " <error sym=\"" + symbol + "\" amount=\"" + to_string(abs(stof(amount))) + "\" limit=\"" + price_limit + "\"> Account ID cannot be negative. </error>\n";
      return;
  
    }

    sql = "SELECT COUNT(*) FROM ACCOUNT WHERE ACCOUNT_ID = " + W.quote(account_id) + " ;";
    R = W.exec(sql);
    res = R.begin();
    if(res[0].as<long long>() == 0){

      response += " <error sym=\"" + symbol + "\" amount=\"" + to_string(abs(stof(amount))) + "\" limit=\"" + price_limit + "\"> Account ID doesn't exist. </error>\n";
      return;
  
    }

     
    float temp_amount = stof(amount);
    if(temp_amount == 0){
      response += " <error sym=\"" + symbol + "\" amount=\"" + to_string(abs(stof(amount))) + "\" limit=\"" + price_limit + "\"> Invalid Share amount. </error>\n";
      return;

    }

    //check if the symbol is valid
    
    int i = 0;
    while(symbol[i]){
      
      if(!isalpha(symbol[i]) && !isdigit(symbol[i])){
	response += " <error sym=\"" + symbol + "\" amount=\"" + to_string(abs(stof(amount))) + "\" limit=\"" + price_limit + "\"> Symbol should be alphanumeric. </error>\n";
	return;
      }
      i++;
    }

    //Check if the symbol exists and is owned by the account ID specified  
    if(temp_amount < 0) {
    sql = "SELECT COUNT(*) FROM SHARES WHERE SHARES.SYMBOL = " + W.quote(symbol) + " AND SHARES.ACCOUNT_ID = " + W.quote(account_id) + " ;";
    R = W.exec(sql);
    res = R.begin();
    if(res[0].as<int>() != 1){
      response += " <error sym=\"" + symbol + "\" amount=\"" + to_string(abs(stof(amount))) + "\" limit=\"" + price_limit + "\"> Either the symbol doesn't exist or the account ID doesn't own the specified share  </error>\n";
      
      return;
      
    }
   }

    //if trying to sell verify the owner has the required number of shares
    if(temp_amount < 0){
    sql = "SELECT AMOUNT FROM SHARES WHERE SHARES.SYMBOL = " + W.quote(symbol) + " AND SHARES.ACCOUNT_ID = " + W.quote(account_id) + " ;";
    R = W.exec(sql);
    res = R.begin();
    if(abs(res[0].as<float>()) < abs(temp_amount)){
       response += " <error sym=\"" + symbol + "\" amount=\"" + to_string(abs(stof(amount))) + "\" limit=\"" + price_limit + "\"> The seller doesn't possess enough shares to sell.  </error>\n";
       return;
      
    }

    }


    //Trying to buy, verify he has enough money to buy
    if(temp_amount > 0){
    
      float required_amount = abs(stof(amount)) * stof(price_limit);  
      sql = "SELECT BALANCE FROM ACCOUNT WHERE ACCOUNT.ACCOUNT_ID = " + W.quote(account_id) + " ;";
      R = W.exec(sql);
      res = R.begin();
      if(res[0].as<float>() < required_amount){
       response += " <error sym=\"" + symbol + "\" amount=\"" + to_string(abs(stof(amount))) + "\" limit=\"" + price_limit + "\"> The buyer doesn't have enough balance to buy required number of shares.  </error>\n";
       return;
      }

    }

      //Now update the open order table

      std::time_t result = std::time(nullptr);
      sql = "INSERT INTO OPEN_ORDERS (ACCOUNT_ID, ORDER_ID, SYMBOL, AMOUNT, PRICE_LIMIT, TIME) VALUES (";
      sql += W.quote(account_id) + ", ";
      sql += W.quote(order_id) + ", ";
      sql += W.quote(symbol) + ", ";
      sql += W.quote(amount) + ", ";
      sql += W.quote(price_limit) + ", ";
      sql += W.quote(result)+ ");";
      W.exec(sql);
      response += " <opened sym=\"" + symbol  + "\" amount=\""  + to_string(abs(stof(amount))) + "\" limit=\""  + price_limit +  "\" id =\"" +  order_id + "\"/>\n";
      

      //Update the balance of the buyer
      if(temp_amount > 0){

	float required_amount = abs(stof(amount)) * stof(price_limit);
	sql = "SELECT BALANCE FROM ACCOUNT WHERE ACCOUNT.ACCOUNT_ID = " + W.quote(account_id) + " ;";
	R = W.exec(sql);
	res = R.begin();
	float player_balance = res[0].as<float>();
	float new_balance = player_balance - required_amount;
	sql = "UPDATE ACCOUNT SET BALANCE = " + W.quote(new_balance) + " WHERE ACCOUNT.ACCOUNT_ID = " + W.quote(account_id) + " ; ";
        W.exec(sql);

      }

      //Deduct shares from seller
      if( temp_amount < 0){
	
	float current_shares;
	sql = "SELECT AMOUNT FROM SHARES WHERE SHARES.ACCOUNT_ID = " + W.quote(account_id) + " AND SHARES.SYMBOL = " + W.quote(symbol) + " ;";
	R = W.exec(sql);
	res = R.begin();
	current_shares = res[0].as<float>();
	float new_shares = current_shares - abs(stof(amount));
	sql = "UPDATE SHARES SET AMOUNT = " + W.quote(new_shares) + " WHERE SHARES.ACCOUNT_ID = " + W.quote(account_id) + " AND SHARES.SYMBOL = " + W.quote(symbol) + " ;";
        W.exec(sql);
      }

      W.commit();
      //Now we can match orders
      match_order(account_id, order_id, symbol, amount, price_limit, response, C);

   }

  catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return ;
  }

}



void match_order(string account_id, string order_id, string symbol, string amount, string price_limit, string & response , connection * C){


  work W(*C);
  result R;
  string sql;
  result::const_iterator res;

  try{

    //user wants to buy
    if(stof(amount) > 0){

      
    long long buyer = stoll(account_id);
    long long incoming_order_id = stoll(order_id);
    float amount_incoming = stof(amount);

      
     sql = "SELECT * FROM OPEN_ORDERS WHERE SYMBOL = " + W.quote(symbol) + " AND PRICE_LIMIT <=  " + W.quote(price_limit) + " AND AMOUNT < 0 AND ACCOUNT_ID != " + W.quote(account_id) + " ORDER BY PRICE_LIMIT ASC, TIME ASC ;";
      R = W.exec(sql);
      res = R.begin();
      if(res == R.end()){
	return;
      }

      else {

	for (res = R.begin(); res != R.end(); res++){

	long long seller_account_id = res[0].as<long long>();
        long long seller_order_id = res[1].as<long long>();
	float selling_amount = abs(res[3].as<float>());
	float selling_price_limit = res[4].as<float>();
	result new_result;
	result::const_iterator new_res;
	float execution_price;
	float sold_amount;
	float money_made;
	



	
	//perfect scenario
	if(selling_amount == amount_incoming){

	  execution_price = selling_price_limit;
	  sold_amount = amount_incoming;
	  money_made = execution_price * sold_amount;
	 }

	else if (selling_amount < amount_incoming)  {

	  
	  execution_price = selling_price_limit;
	  sold_amount =  selling_amount;
	  money_made = execution_price * sold_amount;
	}


	else {

	  execution_price = selling_price_limit;
	  sold_amount =  amount_incoming;
	  money_made = execution_price * sold_amount;

          }



	  //Pay the seller
	  sql = "SELECT BALANCE FROM ACCOUNT WHERE ACCOUNT_ID = " + W.quote(seller_account_id) + " ;";
	  new_result = W.exec(sql);
	  new_res = new_result.begin();
	  float previous_balance = new_res[0].as<float>();
	  float new_balance = previous_balance + money_made;
	  sql = "UPDATE ACCOUNT SET BALANCE = " + W.quote(new_balance) + " WHERE ACCOUNT_ID = " + W.quote(seller_account_id) + " ;";
	  W.exec(sql);
	  
	  
	  

	  //Create position for buyer if not there
	  sql = "SELECT COUNT(*) FROM SHARES WHERE SHARES.ACCOUNT_ID = " + W.quote(buyer) + " AND  SHARES.SYMBOL = " + W.quote(symbol) + " ;";
	  new_result = W.exec(sql);
	  new_res = new_result.begin();
	  if(new_res[0].as<int>() == 0){

	   sql = "INSERT INTO SHARES (SYMBOL, ACCOUNT_ID, AMOUNT) VALUES (";
	   sql += W.quote(symbol) + ", ";
	   sql += W.quote(buyer) + " , ";
	   sql += W.quote(sold_amount)  + ");";
	   W.exec(sql);

	   }

	  else{ //Update if it is already there

	    sql = "SELECT AMOUNT FROM SHARES WHERE SHARES.ACCOUNT_ID = " + W.quote(buyer) + " AND SHARES.SYMBOL = " + W.quote(symbol) + " ;";
	    new_result = W.exec(sql);
	    new_res = new_result.begin();
	    float previous_amount = new_res[0].as<float>();
	    float new_amount = previous_amount + sold_amount;
	    
	    sql = "UPDATE SHARES SET AMOUNT = " + W.quote(new_amount) + " WHERE ACCOUNT_ID = " + W.quote(buyer) + " AND SYMBOL = " + W.quote(symbol) +  " ;";
	    W.exec(sql);

	  }


	  //Now we need to update the execute_shares table
	   std::time_t time_result = std::time(nullptr);
	   //Lastly, update the executes share table
	   sql = "INSERT INTO EXECUTED_SHARES (SYMBOL, ID, PRICE, AMOUNT, TIME) VALUES (";
	   sql += W.quote(symbol) + ", ";
	   sql += W.quote(order_id) + " , ";
	   sql += W.quote(execution_price) + ",";
	   sql += W.quote(sold_amount) + " , ";
	   sql += W.quote(time_result) + ");";
	   W.exec(sql);
	   
	  

	  //Now we need to update the order table
	  if(selling_amount == amount_incoming){

	  sql = "DELETE FROM OPEN_ORDERS WHERE ACCOUNT_ID = " + W.quote(buyer) + " AND ORDER_ID = " + W.quote(incoming_order_id) + " ;";
	  W.exec(sql);
	  sql = "DELETE FROM OPEN_ORDERS WHERE ACCOUNT_ID = " + W.quote(seller_account_id) + " AND ORDER_ID = " + W.quote(seller_order_id) + " ;";
	  W.exec(sql);
	  break;
	  }

	  else if(selling_amount < amount_incoming){
	    
	  sql = "DELETE FROM OPEN_ORDERS WHERE ACCOUNT_ID = " + W.quote(seller_account_id) + " AND ORDER_ID = " + W.quote(seller_order_id) + ";";
	  W.exec(sql);
	  float new_buyer_amount = amount_incoming - selling_amount;
	  sql = "UPDATE OPEN_ORDERS SET AMOUNT = " + W.quote(new_buyer_amount) + "WHERE ACCOUNT_ID = " + W.quote(buyer) + " AND ORDER_ID = " + W.quote(order_id) + " ;";
	  W.exec(sql);
	  amount_incoming = amount_incoming - selling_amount;
	  }

	  else if(selling_amount > amount_incoming){

	  sql = "DELETE FROM OPEN_ORDERS WHERE ACCOUNT_ID = " + W.quote(buyer) + " AND ORDER_ID = " + W.quote(order_id);
	  W.exec(sql);
	  float new_seller_amount = - (selling_amount - amount_incoming);
	  sql = "UPDATE OPEN_ORDERS SET AMOUNT = " + W.quote(new_seller_amount) + "WHERE ACCOUNT_ID = " + W.quote(seller_account_id) + " AND ORDER_ID = " + W.quote(seller_order_id) + " ;";
          W.exec(sql);
          break;
	  
	  }
	  
	}//for loop ends      
    

      } //else

    } //Buying position


    if(stof(amount) < 0){

    long long seller_account_id = stoll(account_id);
    long long incoming_order_id = stoll(order_id);
    float amount_incoming = abs(stof(amount));

      
     sql = "SELECT * FROM OPEN_ORDERS WHERE SYMBOL = " + W.quote(symbol) + " AND PRICE_LIMIT >=  " + W.quote(price_limit) + " AND AMOUNT > 0 AND ACCOUNT_ID != " + W.quote(seller_account_id) + " ORDER BY PRICE_LIMIT DESC, TIME ASC ;";

      R = W.exec(sql);
      res = R.begin();

      if(res == R.end()){
	return;
      }

      else {

	for (res = R.begin(); res != R.end(); res++){

	long long buyer_account_id = res[0].as<long long>();
        long long buyer_order_id = res[1].as<long long>();
	float buyer_amount =  (res[3].as<float>());
	float buyer_price_limit = res[4].as<float>();


	result new_result;
	result::const_iterator new_res;
	float execution_price;
	float sold_amount;
	float money_made;
       
	//perfect scenario
	if(buyer_amount == amount_incoming){

	  execution_price = buyer_price_limit;
	  sold_amount = buyer_amount;
	  money_made = execution_price * sold_amount;

	}

	else if (buyer_amount < amount_incoming)  {

	  
	  execution_price = buyer_price_limit;
	  sold_amount =  buyer_amount;
	  money_made = execution_price * sold_amount;
	}


	else {

	  execution_price = buyer_price_limit;
	  sold_amount =  amount_incoming;
	  money_made = execution_price * sold_amount;

          }



	  //Pay the seller
	  sql = "SELECT BALANCE FROM ACCOUNT WHERE ACCOUNT_ID = " + W.quote(seller_account_id) + " ;";
	  new_result = W.exec(sql);
	  new_res = new_result.begin();
	  float previous_balance = new_res[0].as<float>();
	  float new_balance = previous_balance + money_made;
	  sql = "UPDATE ACCOUNT SET BALANCE = " + W.quote(new_balance) + " WHERE ACCOUNT_ID = " + W.quote(seller_account_id) + " ;";
	  W.exec(sql);
	  
	  
	  

	  //Create position for buyer if not there
	  sql = "SELECT COUNT(*) FROM SHARES WHERE SHARES.ACCOUNT_ID = " + W.quote(buyer_account_id) + " AND  SHARES.SYMBOL = " + W.quote(symbol) + " ;";
	  new_result = W.exec(sql);
	  new_res = new_result.begin();
	  if(new_res[0].as<int>() == 0){

	   sql = "INSERT INTO SHARES (SYMBOL, ACCOUNT_ID, AMOUNT) VALUES (";
	   sql += W.quote(symbol) + ", ";
	   sql += W.quote(buyer_account_id) + " , ";
	   sql += W.quote(sold_amount)  + ");";
	   W.exec(sql);

	   }

	  else{ //Update if it is already there

	    sql = "SELECT AMOUNT FROM SHARES WHERE SHARES.ACCOUNT_ID = " + W.quote(buyer_account_id) + " AND SHARES.SYMBOL = " + W.quote(symbol) + " ;";
	    new_result = W.exec(sql);
	    new_res = new_result.begin();
	    float previous_amount = new_res[0].as<float>();
	    float new_amount = previous_amount + sold_amount;
	    
	    sql = "UPDATE SHARES SET AMOUNT = " + W.quote(new_amount) + " WHERE ACCOUNT_ID = " + W.quote(buyer_account_id) + " AND SYMBOL = " + W.quote(symbol) +  " ;";
	    W.exec(sql);

	  }

	  //Now we need to update the execute_shares table
	   std::time_t time_result = std::time(nullptr);
	   //Lastly, update the executes share table
	   sql = "INSERT INTO EXECUTED_SHARES (SYMBOL, ID, PRICE, AMOUNT, TIME) VALUES (";
	   sql += W.quote(symbol) + ", ";
	   sql += W.quote(order_id) + " , ";
	   sql += W.quote(execution_price) + ",";
	   sql += W.quote(sold_amount) + " , ";
	   sql += W.quote(time_result) + ");";
	   W.exec(sql);
	  
	  
	  

	  //Now we need to update the order table
	  if(buyer_amount == amount_incoming){

	  sql = "DELETE FROM OPEN_ORDERS WHERE ACCOUNT_ID = " + W.quote(buyer_account_id) + " AND ORDER_ID = " + W.quote(buyer_order_id) + " ;";
	  W.exec(sql);
	  sql = "DELETE FROM OPEN_ORDERS WHERE ACCOUNT_ID = " + W.quote(seller_account_id) + " AND ORDER_ID = " + W.quote(incoming_order_id) + " ;";
	  W.exec(sql);

	  break;

	  }

	  else if(buyer_amount < amount_incoming){

	  //cout << "Hello" << endl;
	  sql = "DELETE FROM OPEN_ORDERS WHERE ACCOUNT_ID = " + W.quote(buyer_account_id) + " AND ORDER_ID = " + W.quote(buyer_order_id) + ";";
	  W.exec(sql);
	  float new_seller_amount = - (amount_incoming - buyer_amount);
	  sql = "UPDATE OPEN_ORDERS SET AMOUNT = " + W.quote(new_seller_amount) + "WHERE ACCOUNT_ID = " + W.quote(seller_account_id) + " AND ORDER_ID = " + W.quote(incoming_order_id) + " ;";
	  W.exec(sql);
	  amount_incoming = amount_incoming - buyer_amount;
	  }

	  else if(buyer_amount > amount_incoming){

	  sql = "DELETE FROM OPEN_ORDERS WHERE ACCOUNT_ID = " + W.quote(seller_account_id) + " AND ORDER_ID = " + W.quote(incoming_order_id);
	  W.exec(sql);
	  float new_buyer_amount = (buyer_amount - amount_incoming);
	  sql = "UPDATE OPEN_ORDERS SET AMOUNT = " + W.quote(new_buyer_amount) + "WHERE ACCOUNT_ID = " + W.quote(buyer_account_id) + " AND ORDER_ID = " + W.quote(buyer_order_id) + " ;";
          W.exec(sql);
	  break;
	  }


	}//for loop ends      
    

      } //else

    } //Buying position

}//try


    catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return ;
    }

  
   W.commit();

}



void handle_share_creation(string & response, string symbol, string id, string amount, connection * C){


  work W(*C);

  try {

    string sql;
    

    int i = 0;
    while(symbol[i]){

      if(!isalpha(symbol[i]) && !isdigit(symbol[i])){
	response += " <error sym = \"" + symbol + " id = \"" +  id + "\">"  +  " The symbol of the share should be alphanumeric. " + "</error>\n"; 
        return;
      }
      i++;
    }


       long long num = stoll(id);
       if( num < 0){
	 response += " <error sym = \"" + symbol + " id = \"" + id + "\">" +  "The account id cannot be negative. " + "</error>\n";
       return;

       }

       float temp = stof(amount);
       if( temp <= 0){
	 response += " <error sym = \"" + symbol + " id = \"" + id + "\">"  +  "Negative quanitity of shares is not allowed. " + "</error>\n";
       return;

       }
     
   
    

    //Need to check if the Account ID exits
    sql = "SELECT COUNT(*) FROM ACCOUNT AS A WHERE A.ACCOUNT_ID = " + W.quote(id) + " ;";
    result R = W.exec(sql);
    result::const_iterator res = R.begin();
    if(res[0].as<int>() == 0){

    sql = "INSERT INTO ACCOUNT (ACCOUNT_ID, BALANCE) VALUES (" +  W.quote(id) + ", " +  "0.00" + ");";
    W.exec(sql);
    }

    //Now insert into the Shares Table
    //Need to check whether the symbol already exists
    sql = "SELECT COUNT(*) FROM SHARES AS S WHERE S.ACCOUNT_ID = " + W.quote(id) + " AND S.SYMBOL = "  + W.quote(symbol) + " ;";
    R = W.exec(sql);
    res = R.begin();
    if(res[0].as<int>() == 1){

      sql = "SELECT S.AMOUNT  FROM SHARES AS S WHERE S.ACCOUNT_ID = " + W.quote(id) + " AND S.SYMBOL = "  + W.quote(symbol) + " ;";
      R = W.exec(sql);
      float current_amount = res[0].as<float>();
      float new_amount = current_amount + stof(amount);
      sql = "UPDATE SHARES SET AMOUNT = " + W.quote(new_amount) +  " WHERE SHARES.ACCOUNT_ID = " + W.quote(id) + " AND SHARES.SYMBOL = "  + W.quote(symbol) + " ;";
      W.exec(sql);
      response += " <created sym = \"" +  symbol  + "\" " + "id = \"" + id + "\"/>\n";
    }
    else{
      sql = "INSERT INTO SHARES (SYMBOL, ACCOUNT_ID, AMOUNT) VALUES (";
      sql += W.quote(symbol) + ", ";
      sql += W.quote(id) + " , ";
      sql += W.quote(amount)  + ");";

      W.exec(sql);
      response += " <created sym = \"" +  symbol  + "\" " + "id = \"" + id + "\"/>\n";
      
    }

 }

  catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return ;
  }

  W.commit();

}


void handle_account_creation(string balance, string id, string & response, connection * C){


  try{

  double bal = stold(balance);
  if( bal <= 0){
       response += " <error id = \"" + id + "\"> Balance cannot be negative. </error> \n";
       return;
 
  }

  long long num = stoll(id);;
  if( num <= 0){
       response += " <error id = \"" + id + "\"> Account ID cannot be negative. </error> \n";
       return;
 
  }

  work W(*C);
 //Check whether account exists or not
 string  sql = "SELECT COUNT(*) "
                "FROM ACCOUNT AS A "
                "WHERE A.ACCOUNT_ID = " + W.quote(id) + " ;";

  result R = W.exec(sql);
  result::const_iterator res = R.begin();
  
  if(res[0].as<int>() == 1){
    response += " <error id = \"" + id + "\"> Account already exists </error>\n";
    return ;
  }
  else{

    sql = "INSERT INTO ACCOUNT (ACCOUNT_ID, BALANCE) VALUES (";
    sql += W.quote(id) + ", ";
    sql += W.quote(balance) + ");";
    W.exec(sql);
    W.commit();

    response += " <created id = \"" + id + "\"/>\n"; 
  }

  }


  catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return ;
  }

}

void drop_all_tables(connection * C){

  string sql;
  work W(*C);
  sql = "DROP TABLE IF EXISTS CLOSED_ORDERS, ACCOUNT, SHARES, OPEN_ORDERS, EXECUTED_SHARES ;";
  W.exec(sql);
  W.commit();
  // cout << "drop all tables" << endl;
}


void create_account_table(connection * C){

   string sql;
   work W(*C);
   sql = " CREATE TABLE ACCOUNT (" 
         " ACCOUNT_ID       BIGINT                NOT NULL, " 
         " BALANCE          NUMERIC(30,2)         NOT NULL         DEFAULT 0, "
         " PRIMARY KEY(ACCOUNT_ID), "
         " CHECK(BALANCE >= 0));";
   W.exec(sql);
   // cout << "create account table" << endl;
   W.commit();
}


void create_shares_table(connection * C){


  string sql;
  work W(*C);
  sql = "CREATE TABLE SHARES ("
        "SYMBOL        VARCHAR(100) NOT NULL,"
        "ACCOUNT_ID    BIGINT       NOT NULL, "
        "AMOUNT        FLOAT        NOT NULL, "
        "PRIMARY KEY(ACCOUNT_ID, SYMBOL),"
        "CHECK(AMOUNT >= 0));";
 
  W.exec(sql);
  W.commit();
}


void executed_shares_table(connection * C){


  string sql;
  work W(*C);
  sql = "CREATE TABLE EXECUTED_SHARES ("
        "SYMBOL        VARCHAR(100) NOT NULL,"
        "ID            BIGINT       NOT NULL, "
        "PRICE         FLOAT        NOT NULL, "
        "AMOUNT        FLOAT        NOT NULL, "
        "TIME          BIGINT       NOT NULL);";

 
  W.exec(sql);
  W.commit();

}



void create_open_orders(connection * C){

  string sql;
  work W(*C);
  sql = "CREATE TABLE OPEN_ORDERS("
        "ACCOUNT_ID     BIGINT                NOT NULL,"
        "ORDER_ID       BIGINT                NOT NULL,"
        "SYMBOL         VARCHAR(50)           NOT NULL,"
        "AMOUNT         FLOAT                 NOT NULL,"
        "PRICE_LIMIT    FLOAT                 NOT NULL,"
        "TIME           BIGINT                NOT NULL, "
        "PRIMARY KEY(ACCOUNT_ID, ORDER_ID),"
        "CHECK(PRICE_LIMIT > 0));";

  W.exec(sql);
  W.commit();
}


void create_closed_orders(connection * C){

  string sql;
  work W(*C);
    sql = "CREATE TABLE CLOSED_ORDERS("
          "ACCOUNT_ID     BIGINT                NOT NULL,"
          "ORDER_ID       BIGINT                NOT NULL,"
          "SYMBOL         VARCHAR(50)           NOT NULL,"
          "AMOUNT         FLOAT                 NOT NULL,"
          "PRICE_LIMIT    FLOAT                 NOT NULL,"
          "TIME           BIGINT                NOT NULL, "
          "PRIMARY KEY(ACCOUNT_ID, ORDER_ID),"
          "CHECK(PRICE_LIMIT > 0));";


  W.exec(sql);
  W.commit();
}


int parse_xml(string & xml){

  stringstream ss;
  size_t position = xml.find("\n");
  if(position == string::npos){
    return -1;
  }

 xml = xml.substr(position+1);
 string original = xml;
 position = xml.find("<create>\n");

 if ((position != string::npos)) {

      xml = xml.substr(position + 9);
      if ((position = xml.find("<transactions")) != string::npos)
	return -1; //Cannot have a transaction inside create
    
      if ((position = xml.find("<create>\n")) != string::npos) 
              return -1; //Cannot have two create
  }

  else if ((position = xml.find("<transactions")) != string::npos) {
     
       xml = xml.substr(position + 13);
       position = xml.find("<transactions");
       if ((position != string::npos)) {
        return -1; // cannot have more than one <transactions>
      }

       
      if ((position = xml.find("<create>\n")) != string::npos) 
              return -1; //Cannot have two creat
       
    } 


  if (xml.find("<symbol") == string::npos &&  xml.find("<account") == string::npos && xml.find("<order") == string::npos && xml.find("<query") == string::npos && xml.find("<cancel") == string::npos)
 {
      return -1; 
  }

    
    original.erase(std::remove(original.begin(), original.end(), '\n'), original.end());  //Remove all \n for parsing 
    xml = original;
    //cout << xml << endl;
    return 1;

}



void parse_create_xml(xml_node & node, string & response, connection * C){


  // vector <vector<string>> accounts;
  // vector <vector<string>> shares; 
  string id;
  string symbol;
  string balance;
  string amount;
  
  

  for (xml_node_iterator it  = node.begin(); it!= node.end(); ++it)
{
  
  //cout << it->name() << endl;
  xml_node current = *it;
  string my_str(it->name());
  
  if(my_str == "account" && it->attribute("id") && it->attribute("balance")){

    id = it->attribute("id").value();
    balance = it->attribute("balance").value();
    handle_account_creation(balance, id, response, C);
    
  }

  if(my_str == "symbol" && it->attribute("sym")){

    symbol = it->attribute("sym").value();
    for (xml_node_iterator to = current.begin(); to != current.end(); to++){

      //cout << symbol << endl;
      string my_str(to->name());
      string temp_str(to->child_value());
      if(my_str == "account" && to->attribute("id") && temp_str != "") {
	  id = to->attribute("id").value();
	  amount = to->child_value();
	  handle_share_creation(response, symbol, id, amount, C);
      }
    }
  }
 }
}


void handle_request_all(string  xml, string  response, ConnectionPool * pool, Socket && clientSocket) {

  while (pool->getPoolSize() == 0) {}
  shared_ptr<connection> ptr = pool->getConnect();
  connection * C = ptr.get();

  if(parse_xml(xml) == -1){
    response += "<results>\n"
                " <error> Invalid Request. Please check the syntax. </error>\n"
               "</results>\n";
    //cout << response << endl;
    pool->retConnect(ptr);
    //cout << "----------------------------------" << endl;
    clientSocket.sendMessage(response);
    return;
  }
  
  //int response_status;
  response += "<results>\n";

  xml_document doc;
  xml_parse_result result = doc.load_string(xml.c_str(), sizeof(xml.c_str()));
  if (!result){
    cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
    response += " <error> Invalid Request. Parsing error. Please check syntax. </error>\n"
                "</results>\n";
    pool->retConnect(ptr);
    //cout << "----------------------------------" << endl;
    clientSocket.sendMessage(response);
    return;
  }

  xml_node root = doc.document_element();
  const char_t * node_name = root.name();

  if (strcmp(node_name, "create") == 0){
    parse_create_xml(root, response, C);
    response.append("</results>\n");
    //cout << "response in handle requests all " << endl;
    //cout << response << endl;
    pool->retConnect(ptr);
    //cout << "----------------------------------" << endl;
    clientSocket.sendMessage(response);
    return;
  }
  else if (strcmp(node_name, "transactions") == 0) {

    Transaction transaction(xml);
    string account_id = transaction.getAccountId();
     
    if (transaction.hasOrders()) {
      // cout << "Entered" << endl;
      const vector<Transaction::Order> orders = transaction.getOrders();
      for (size_t  i = 0 ; i < orders.size();  i++) {
	
	add_order(account_id, to_string(global_order_id), orders[i].getSym(), orders[i].getAmount(), orders[i].getLimit(), response , C);
	mutex_t.lock();
	global_order_id++;
	mutex_t.unlock();
      }
    }
   
    if (transaction.hasCancels()) {
      const vector<Transaction::Cancel> cancels = transaction.getCancels();
      for (size_t i = 0; i < cancels.size(); i++) {
	string cancel_id = cancels[i].getTransId();
	response += " <canceled id=\"";
	response += cancel_id;
	response += "\">\n";
	cancel_order(cancel_id, account_id, response, C);
      }

    }

    if (transaction.hasQueries()) {
      const vector<Transaction::Query> queries = transaction.getQueries();
      for (size_t i = 0 ; i < queries.size(); i++) {
	string query_id = queries[i].getTransId();
	response += " <status id=\"";
	response += query_id;
	response += "\">\n";
	query_order(query_id, account_id, response, C);
      }
    }
 

    
    // cout << response << endl;
    response.append("</results>\n");
    pool->retConnect(ptr);
    // cout << "----------------------------------" << endl;
    clientSocket.sendMessage(response);
    return;
  }  
}
    


  

