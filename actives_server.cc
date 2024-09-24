#include "actives_server.h"

void get_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr) {
    spdlog::info("get_actives enabled");
    svr_ptr -> Get("/all_actives", [pool_ptr](const httplib::Request & /*req*/, httplib::Response &res) {
        res.set_content(cp::serialize(all_public(pool_ptr)), "application/json");
    });
}

void get_active(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr) {
    spdlog::info("get_active enabled");
    svr_ptr -> Get("/active", [pool_ptr](const httplib::Request & req, httplib::Response &res) {
        if(!req.get_header_value("id").empty()) {
            res.set_content(cp::serialize(active(pool_ptr, stoi(req.get_header_value("id")))), "application/json");
        }
        else if(!req.get_header_value("ticker").empty()) {
            res.set_content(cp::serialize(active(pool_ptr, req.get_header_value("ticker"))), "application/json");
        } else {
            res.status = 401;
        }
    });
}

void get_active_history(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr) {
    spdlog::info("get_active_history enabled");
    svr_ptr -> Get("/activehistory", [pool_ptr](const httplib::Request & req, httplib::Response &res) {
        if(!req.get_header_value("id").empty()) {
            res.set_content(cp::serialize(active_history(pool_ptr, stoi(req.get_header_value("id")))), "application/json");
        }
        else if(!req.get_header_value("ticker").empty()) {
            res.set_content(cp::serialize(active_history(pool_ptr, req.get_header_value("ticker"))), "application/json");
        } else {
            res.status = 401;
        }
    });
}

void get_user_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr) {
    spdlog::info("get_user_actives enabled");
    svr_ptr -> Get("/useractives", [pool_ptr](const httplib::Request & req, httplib::Response &res) {
        if(!req.get_header_value("userid").empty()) {
            res.set_content(cp::serialize(user_actives(pool_ptr, stoi(req.get_header_value("userid")))), "application/json");
        } else {
            res.status = 401;
        }
    });
}

void post_bid(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr) {
    spdlog::info("post_bid enabled");
    svr_ptr -> Post("/add_bid", [pool_ptr](const httplib::Request & req, httplib::Response &res) {
        rapidjson::Document new_body;
        new_body.Parse(req.body.c_str());

        if(new_body.HasMember("buy") && new_body.HasMember("userid") && new_body.HasMember("activeid") && new_body.HasMember("bidprice") && new_body.HasMember("ammount")) {
            std::string buy = new_body["buy"].GetString(), userId = new_body["userid"].GetString(), activeId = new_body["activeid"].GetString(), bidPrice = new_body["bidprice"].GetString(), ammount = new_body["ammount"].GetString();
            res.set_content(add_bid(pool_ptr, buy, stoi(userId), stoi(activeId), stoi(bidPrice), stoi(ammount)), "text/plain");
            res.status = 200;
        } else {
            res.status = 401;
        }
    });
}

void enable_all_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr) {
    get_actives(svr_ptr, pool_ptr);
    get_active(svr_ptr, pool_ptr);
    get_active_history(svr_ptr, pool_ptr);
    get_user_actives(svr_ptr, pool_ptr);
    post_bid(svr_ptr, pool_ptr);
}