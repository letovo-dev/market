#pragma once
#include <jwt-cpp/jwt.h>
#include <restinio/all.hpp>
#include <pqxx/pqxx>
#include <unordered_set>
// #include "spdlog/spdlog.h"
#include "rapidjson/document.h"
#include "asio/ip/detail/endpoint.hpp"
#include "../basic/pqxx_cp.h"
#include "../basic/hash.h"
#include "../basic/auth.h"
#include "../basic/url_parser.h"
#include "../basic/comment.h"
#include <vector>

namespace transactions {
    bool transfer(std::string sender_username, std::string receiver_username, int amount, std::shared_ptr<cp::ConnectionsManager> pool_ptr);

    int get_balance(std::string username, std::shared_ptr<cp::ConnectionsManager> pool_ptr);

    pqxx::result get_transactions(std::string username, std::shared_ptr<cp::ConnectionsManager> pool_ptr);
} // namespace transactions

namespace transactions::server {
    void transfer(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

    void get_balance(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

    void get_transactions(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);
} // namespace transactions::server
