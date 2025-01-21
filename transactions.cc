#include "./transactions.h"

namespace transactions {

    int get_balance(std::string username, std::shared_ptr<cp::ConnectionsManager> pool_ptr){
        auto con = std::move(pool_ptr->getConnection());

        std::vector<std::string> params = {username};

        pqxx::result result = con->execute_params("SELECT balance FROM \"user\" WHERE username=($1);", params);
        
        pool_ptr->returnConnection(std::move(con));

        if(result.empty()) {
            return -1;
        }
        return result[0]["balance"].as<int>();
    }

    bool transfer(std::string sender_username, std::string receiver_username, int amount, std::shared_ptr<cp::ConnectionsManager> pool_ptr) {
        int balance = get_balance(sender_username, pool_ptr);
        bool flag = amount < 0;
        if(auth::is_admin_by_uname(sender_username, pool_ptr)) {
            balance = 999999999;
            flag = false;
        }
        if (balance < amount || flag) {
            return false;
        }

        auto con = std::move(pool_ptr->getConnection());
        std::vector<std::string> params = {std::to_string(amount), sender_username};

        con->execute_params("UPDATE \"user\" SET balance=balance-($1) WHERE username=($2);", params, true);
        params[1] = receiver_username;
        con->execute_params("UPDATE \"user\" SET balance=balance+($1) WHERE username=($2);", params, true);
        params = {sender_username, receiver_username, std::to_string(amount)};
        con->execute_params("INSERT INTO \"transactions\" (sender, receiver, amount) VALUES($1, $2, $3);", params, true);

        pool_ptr->returnConnection(std::move(con));

        return true;
    }

    pqxx::result get_transactions(std::string username, std::shared_ptr<cp::ConnectionsManager> pool_ptr){
        auto con = std::move(pool_ptr->getConnection());
        std::vector<std::string> params = {username};
        pqxx::result result = con->execute_params("SELECT * FROM \"transactions\" WHERE sender=($1) OR receiver=($1);", params);
        pool_ptr->returnConnection(std::move(con));        
        if(result.empty()) {
            return {};
        }
        return result;
    }
}

namespace transactions::server {
    void transfer(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
        router.get()->http_post("/transactions/send", [pool_ptr, logger_ptr](auto req, auto) {
            rapidjson::Document new_body;
            new_body.Parse(req->body().c_str());

            if (!new_body.HasMember("token") || !auth::is_authed(new_body["token"].GetString(), pool_ptr)) {
                return req->create_response(restinio::status_unauthorized()).done();
            }

            if (new_body.HasMember("receiver") && new_body.HasMember("amount")) {
                std::string sender = auth::get_username(new_body["token"].GetString(), pool_ptr);
                std::string receiver = new_body["receiver"].GetString();
                int amount = new_body["amount"].GetInt();

                if(transactions::transfer(sender, receiver, amount, pool_ptr)) {
                    std::cout << "ok" << std::endl;
                    return req->create_response().set_body("ok").done();
                } else {
                    std::cout << "no money" << std::endl;
                    return req->create_response().set_body(Comment::giveMe().no_money).done();
                }
            } else {
                return req->create_response(restinio::status_non_authoritative_information()).done();
            }
        });
    }

    void get_balance(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
        router.get()->http_get(R"(/transactions/balance/:token([a-zA-Z0-9]+))", [pool_ptr, logger_ptr](auto req, auto) {
            std::string token = url::get_last_url_arg(req->header().path());

            if(token.empty() || token == "balance") {
                return req->create_response(restinio::status_non_authoritative_information()).done();
            }

            if(!auth::is_authed(token, pool_ptr)) {
                return req->create_response(restinio::status_unauthorized()).done();
            }

            std::string username = auth::get_username(token, pool_ptr);

            return req->create_response().set_body(std::to_string(transactions::get_balance(username, pool_ptr))).done();
            
        });
    }

    void get_transactions(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
        router.get()->http_get(R"(/transactions/get/:token([a-zA-Z0-9]+))", [pool_ptr, logger_ptr](auto req, auto) {
            std::string token = url::get_last_url_arg(req->header().path());

            if(token.empty() || token == "get") {
                return req->create_response(restinio::status_non_authoritative_information()).done();
            }

            if(!auth::is_authed(token, pool_ptr)) {
                return req->create_response(restinio::status_unauthorized()).done();
            }

            std::string username = auth::get_username(token, pool_ptr);

            return req->create_response().set_body(cp::serialize(transactions::get_transactions(username, pool_ptr))).done();
        });
    }
}