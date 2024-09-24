/**
 * @file actives_server.h
 * @brief Header file for handling active market operations on the server.
 *
 * This file contains function declarations for managing active market operations
 * such as retrieving actives, retrieving a specific active, retrieving active history,
 * retrieving user actives, posting bids, and enabling all actives. These functions
 * utilize the httplib server and a connection pool for database interactions.
 *
 * Dependencies:
 * - auth.h: Authentication handling.
 * - httplib.h: HTTP server library.
 * - pqxx_cp.h: Connection pool for PostgreSQL.
 * - pqxx/pqxx: PostgreSQL C++ library.
 * - actives_db.h: Database operations for actives.
 *
 * Functions:
 * - void get_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);
 * - void get_active(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);
 * - void get_active_history(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);
 * - void get_user_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);
 * - void post_bid(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);
 * - void enable_all_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);
 * 
 * !! naming convention: fisrt word is the http method, second word is the object of the operation
 */

#include "../auth.h"
#include <httplib.h>
#include "../pqxx_cp.h"
#include <pqxx/pqxx>
#include "actives_db.h"


void get_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);

void get_active(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);

void get_active_history(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);

void get_user_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);

void post_bid(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);

void enable_all_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr);