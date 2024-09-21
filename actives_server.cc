#include "actives_server.h"

void get_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr) {
    svr_ptr -> Get("/all_actives", [pool_ptr](const httplib::Request & /*req*/, httplib::Response &res) {
        res.set_content(cp::serialize(all_public(pool_ptr)), "application/json");
    });
}

void get_active(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr) {
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

void user_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr) {
    svr_ptr -> Get("/useractives", [pool_ptr](const httplib::Request & req, httplib::Response &res) {
        if(!req.get_header_value("userid").empty()) {
            res.set_content(cp::serialize(user_actives(pool_ptr, stoi(req.get_header_value("userid")))), "application/json");
        } else {
            res.status = 401;
        }
    });
}

void enable_all_actives(std::shared_ptr<httplib::Server> svr_ptr, std::shared_ptr<cp::connection_pool> pool_ptr) {
    get_actives(svr_ptr, pool_ptr);
    get_active(svr_ptr, pool_ptr);
    get_active_history(svr_ptr, pool_ptr);
    user_actives(svr_ptr, pool_ptr);
}