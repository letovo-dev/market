#pragma once

/**
 * @file actives_db.h
 * @brief This file contains function declarations for interacting with the market actives database.
 *
 * Functions:
 * - pqxx::result all_public(std::shared_ptr<cp::ConnectionsManager> pool_ptr);
 *   Retrieves all public actives from the database.
 *
 * - pqxx::result active(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int activeId);
 *   Retrieves a specific active by its ID.
 *
 * - pqxx::result active(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string activeTicker);
 *   Retrieves a specific active by its ticker.
 *
 * - pqxx::result active_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int activeId);
 *   Retrieves the history of a specific active by its ID.
 *
 * - pqxx::result active_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string activeTicker);
 *   Retrieves the history of a specific active by its ticker.
 *
 * - pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId);
 *   Retrieves all actives associated with a specific user by their ID.
 *
 * - pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, int activeId);
 *   Retrieves a specific active associated with a specific user by their user ID and active ID.
 *
 * - pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, std::string activeTicker);
 *   Retrieves a specific active associated with a specific user by their user ID and active ticker.
 *
 * - pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId);
 *   Retrieves the history of all actives associated with a specific user by their ID.
 *
 * - pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, int activeId);
 *   Retrieves the history of a specific active associated with a specific user by their user ID and active ID.
 *
 * - pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, std::string activeTicker);
 *   Retrieves the history of a specific active associated with a specific user by their user ID and active ticker.
 *
 * - std::string add_bid(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string buy, int userId, int activeId, int bidPrice, int amount);
 *   Adds a bid for a specific active by a user. Can be a buy or sell bid.
 */

#include "../basic/pqxx_cp.h"
#include <pqxx/pqxx>
#include <restinio/all.hpp>
#include "../basic/url_parser.h"
#include "../basic/auth.h"


namespace actives {
    pqxx::result all_public(std::shared_ptr<cp::ConnectionsManager> pool_ptr);

    pqxx::result active(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int activeId);

    pqxx::result active(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string activeTicker);

    pqxx::result active_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int activeId);

    pqxx::result active_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string activeTicker);

    pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId);

    pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, int activeId);

    pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, std::string activeTicker);

    pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId);

    pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, int activeId);

    pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, std::string activeTicker);

    std::string add_bid(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string buy, int userId, int activeId, int bidPrice, int ammount);
}

namespace actives::server {
    void all_public(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

    void active(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

    void history(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

    void add_bid(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

}