/**
 * @file actives_db.h
 * @brief This file contains function declarations for interacting with the market actives database.
 * 
 * Functions:
 * - pqxx::result all_public(std::shared_ptr<cp::connection_pool> pool_ptr);
 *   Retrieves all public actives from the database.
 * 
 * - pqxx::result active(std::shared_ptr<cp::connection_pool> pool_ptr, int activeId);
 *   Retrieves a specific active by its ID.
 * 
 * - pqxx::result active(std::shared_ptr<cp::connection_pool> pool_ptr, std::string activeTicker);
 *   Retrieves a specific active by its ticker.
 * 
 * - pqxx::result active_history(std::shared_ptr<cp::connection_pool> pool_ptr, int activeId);
 *   Retrieves the history of a specific active by its ID.
 * 
 * - pqxx::result active_history(std::shared_ptr<cp::connection_pool> pool_ptr, std::string activeTicker);
 *   Retrieves the history of a specific active by its ticker.
 * 
 * - pqxx::result user_actives(std::shared_ptr<cp::connection_pool> pool_ptr, int userId);
 *   Retrieves all actives associated with a specific user by their ID.
 * 
 * - pqxx::result user_actives(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, int activeId);
 *   Retrieves a specific active associated with a specific user by their user ID and active ID.
 * 
 * - pqxx::result user_actives(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, std::string activeTicker);
 *   Retrieves a specific active associated with a specific user by their user ID and active ticker.
 * 
 * - pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId);
 *   Retrieves the history of all actives associated with a specific user by their ID.
 * 
 * - pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, int activeId);
 *   Retrieves the history of a specific active associated with a specific user by their user ID and active ID.
 * 
 * - pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, std::string activeTicker);
 *   Retrieves the history of a specific active associated with a specific user by their user ID and active ticker.
 * 
 * - std::string add_bid(std::shared_ptr<cp::connection_pool> pool_ptr, std::string buy, int userId, int activeId, int bidPrice, int amount);
 *   Adds a bid for a specific active by a user. Can be a buy or sell bid.
 */

#include "../pqxx_cp.h"
#include <pqxx/pqxx>
// #include "spdlog/spdlog.h"


pqxx::result all_public(std::shared_ptr<cp::connection_pool> pool_ptr);

pqxx::result active(std::shared_ptr<cp::connection_pool> pool_ptr, int activeId);

pqxx::result active(std::shared_ptr<cp::connection_pool> pool_ptr, std::string activeTicker);

pqxx::result active_history(std::shared_ptr<cp::connection_pool> pool_ptr, int activeId);

pqxx::result active_history(std::shared_ptr<cp::connection_pool> pool_ptr, std::string activeTicker);

pqxx::result user_actives(std::shared_ptr<cp::connection_pool> pool_ptr, int userId);

pqxx::result user_actives(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, int activeId);

pqxx::result user_actives(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, std::string activeTicker);

pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId);

pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, int activeId);

pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, std::string activeTicker);

std::string add_bid(std::shared_ptr<cp::connection_pool> pool_ptr, std::string buy,int userId, int activeId, int bidPrice, int ammount);