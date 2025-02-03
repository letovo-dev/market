#include "../basic/pqxx_cp.h"
#include "../basic/utils.h"
#include "../basic/auth.h"
#include "transactions.h"
#include "actives.h"
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>

namespace actives::deals {
    class DOM {
        private:
            // contains {price, bid}
            std::map<int, utils::LinkedList> bids;
            void resolve_bids(std::shared_ptr<cp::ConnectionsManager> pool_ptr);

        public:
            DOM(std::shared_ptr<cp::ConnectionsManager> pool_ptr);

            bool add_bid(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int bid_id, std::string user_name, int price, int amount, actives::active_obj act);

            void remove_bid(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int, int);

            void remove_bid(std::shared_ptr<cp::ConnectionsManager> pool_ptr, bid);

            std::vector<bid> users_bids(std::string user_name);

            void start_resolver(std::shared_ptr<cp::ConnectionsManager> pool_ptr);

            std::vector<bid> check_bids();
    };
}

namespace actives::deals::server {
    void add_bid(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr, std::shared_ptr<actives::deals::DOM> dom_ptr);

    void remove_bid(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr, std::shared_ptr<actives::deals::DOM> dom_ptr);

    void users_bids(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr, std::shared_ptr<actives::deals::DOM> dom_ptr);

    void enable_bids(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);
}

// std::string add_bid(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string buy, std::string user_name, int activeId, int bidPrice, int ammount) {
//         std::vector<std::string> params = {buy, user_name, std::to_string(activeId), std::to_string(bidPrice), std::to_string(ammount)};
//         auto con = std::move(pool_ptr->getConnection());
//         std::string message;
//         try {
//             con->execute_params("INSERT INTO \"bids\" (buy, user_name, activeId, bidPrice, ammount) VALUES($1, $2, $3, $4, $5);", params, true);

//             message = "ok";
//         } catch (const char* error_message) {
//             message = error_message;
//         }
//         pool_ptr->returnConnection(std::move(con));
        
//         return message;
//     }

//     void close_bid(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int bidId) {
//         std::vector<int> params = {bidId};
//         auto con = std::move(pool_ptr->getConnection());

//         // sql line

//         pool_ptr->returnConnection(std::move(con));
//     }

// void add_bid(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
//         router.get()->http_post("/actives/add_bid", [pool_ptr, logger_ptr](auto req, auto) {
//             std::string token;
//             try {
//                 token = req -> header().get_field("token");
//             } catch (const std::exception& e) {
//                 return req->create_response(restinio::status_non_authoritative_information()).done();
//             }

//             if (token.empty()) {
//                 return req->create_response(restinio::status_non_authoritative_information()).done();
//             }

//             if (!auth::is_authed(token, pool_ptr)) {
//                 return req->create_response(restinio::status_unauthorized()).done();
//             }

//             rapidjson::Document new_body;
//             new_body.Parse(req->body().c_str());

//             if (new_body.HasMember("buy") && new_body.HasMember("activeId") && new_body.HasMember("bidPrice") && new_body.HasMember("ammount")) {
//                 std::string buy = new_body["buy"].GetString();
//                 std::string user_name = new_body["user_name"].GetString();
//                 int activeId = new_body["activeId"].GetInt();
//                 int bidPrice = new_body["bidPrice"].GetInt();
//                 int ammount = new_body["ammount"].GetInt();

//                 std::string message = actives::add_bid(pool_ptr, buy, user_name, activeId, bidPrice, ammount);

//                 if (message == "ok") {
//                     return req->create_response().set_body("ok").done();
//                 } else {
//                     return req->create_response(restinio::status_internal_server_error()).done();
//                 }
//             } else {
//                 return req->create_response(restinio::status_non_authoritative_information()).done();
//             }
//         });
//     }