#include "actives_server.h"

void get_actives(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::connection_pool> pool_ptr) {
    // spdlog::info("get_actives enabled");
    router.get()->http_get("/all_actives", [pool_ptr](auto req, auto){
        return req->create_response().set_body(cp::serialize(all_public(pool_ptr))).done();
    });
}

void get_active(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::connection_pool> pool_ptr) {
    // spdlog::info("get_active enabled");
    // router -> Get("/active", [pool_ptr](const httplib::Request & req, httplib::Response &res) {
    //     if(!req.get_header_value("id").empty()) {
    //         res.set_content(cp::serialize(active(pool_ptr, stoi(req.get_header_value("id")))), "application/json");
    //     }
    //     else if(!req.get_header_value("ticker").empty()) {
    //         res.set_content(cp::serialize(active(pool_ptr, req.get_header_value("ticker"))), "application/json");
    //     } else {
    //         res.status = 401;
    //     }
    // });
}

// void get_active_history(std::shared_ptr<httplib::Server> router, std::shared_ptr<cp::connection_pool> pool_ptr) {
//     spdlog::info("get_active_history enabled");
//     router -> Get("/activehistory", [pool_ptr](const httplib::Request & req, httplib::Response &res) {
//         if(!req.get_header_value("id").empty()) {
//             res.set_content(cp::serialize(active_history(pool_ptr, stoi(req.get_header_value("id")))), "application/json");
//         }
//         else if(!req.get_header_value("ticker").empty()) {
//             res.set_content(cp::serialize(active_history(pool_ptr, req.get_header_value("ticker"))), "application/json");
//         } else {
//             res.status = 401;
//         }
//     });
// }

// void get_user_actives(std::shared_ptr<httplib::Server> router, std::shared_ptr<cp::connection_pool> pool_ptr) {
//     spdlog::info("get_user_actives enabled");
//     router -> Get("/useractives", [pool_ptr](const httplib::Request & req, httplib::Response &res) {
//         if(!req.get_header_value("userid").empty()) {
//             res.set_content(cp::serialize(user_actives(pool_ptr, stoi(req.get_header_value("userid")))), "application/json");
//         } else {
//             res.status = 401;
//         }
//     });
// }

void post_bid(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::connection_pool> pool_ptr) {
    // spdlog::info("post_bid enabled");
    router.get()->http_post("/add_bid", [pool_ptr](auto req, auto) {
        rapidjson::Document new_body;
        new_body.Parse(req->body().c_str());

        if(new_body.HasMember("buy") && new_body.HasMember("userid") && new_body.HasMember("activeid") && new_body.HasMember("bidprice") && new_body.HasMember("ammount")) {
            std::string buy = new_body["buy"].GetString(), userId = new_body["userid"].GetString(), activeId = new_body["activeid"].GetString(), bidPrice = new_body["bidprice"].GetString(), ammount = new_body["ammount"].GetString();
            return req->create_response().set_body(add_bid(pool_ptr, buy, stoi(userId), stoi(activeId), stoi(bidPrice), stoi(ammount))).done();

            // spdlog::info("new bid by {} added", userId);
        } else {
            return req->create_response(restinio::status_bad_request()).done();
        }
    });
}

void enable_all_actives(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::connection_pool> pool_ptr) {
    get_actives(router, pool_ptr);
    get_active(router, pool_ptr);
    // get_active(router, pool_ptr);
    // get_active_history(router, pool_ptr);
    // get_user_actives(router, pool_ptr);
    post_bid(router, pool_ptr);
}