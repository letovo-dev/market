#include "actives_db.h"


namespace actives {
    pqxx::result all_public(std::shared_ptr<cp::ConnectionsManager> pool_ptr) {
        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute("SELECT * FROM \"active\" WHERE \"ispublic\";");

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result active(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int activeId) {
        std::vector<int> params = {activeId};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT * FROM \"active\" WHERE \"ispublic\" AND activeId = ($1);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result active(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string activeTicker) {
        std::vector<std::string> params = {activeTicker};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT * FROM \"active\" WHERE \"ispublic\" AND activeTicker = ($1);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result active_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int activeId) {
        std::vector<int> params = {activeId};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT ah.* FROM \"activeHistory\" ah JOIN \"active\" a ON ah.activeId = a.activeId WHERE a.ispublic = true AND ah.activeId = ($1);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result active_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string activeTicker) {
        std::vector<std::string> params = {activeTicker};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT ah.* FROM \"activeHistory\" ah JOIN \"active\" a ON ah.activeId = a.activeId WHERE a.ispublic = true AND a.activeTicker = ($1);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId) {
        std::vector<int> params = {userId};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT * FROM usersactives WHERE userId=($1);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, int activeId) {
        std::vector<int> params = {userId, activeId};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT * FROM usersactives WHERE userId=($1) and activeId=($2);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, std::string activeTicker) {
        std::vector<int> params = {userId};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT ua.* FROM usersactives ua JOIN \"active\" a ON ua.activeId = a.activeId WHERE ua.userId=($1) and a.activeTicker=($2);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId) {
        std::vector<int> params = {userId};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT * FROM \"activeHistory\" WHERE userId=($1);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, int activeId) {
        std::vector<int> params = {userId, activeId};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT * FROM \"activeHistory\" WHERE userId=($1) AND activeId = ($2);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, std::string activeTicker) {
        std::vector<int> params = {userId};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT ah.* FROM \"activeHistory\" ah JOIN \"active\" a ON ah.activeId = a.activeId WHERE ah.userId=($1) AND a.activeTicker = ($2);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    std::string add_bid(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string buy, int userId, int activeId, int bidPrice, int ammount) {
        std::vector<std::string> params = {buy, std::to_string(userId), std::to_string(activeId), std::to_string(bidPrice), std::to_string(ammount)};
        auto con = std::move(pool_ptr->getConnection());
        std::string message;
        try {
            con->execute_params("INSERT INTO \"bids\" (buy, userId, activeId, bidPrice, ammount) VALUES($1, $2, $3, $4, $5);", params, true);

            message = "ok";
        } catch (const char* error_message) {
            message = error_message;
        }
        pool_ptr->returnConnection(std::move(con));
        
        return message;
    }
}

namespace actives::server {
    void all_public(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
        router.get()->http_get("/actives/all_actives", [pool_ptr, logger_ptr](auto req, auto) {
            return req->create_response().set_body(cp::serialize(actives::all_public(pool_ptr))).done();
        });
    }

    void active(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr) {
        router.get()->http_get("/actives/active", [pool_ptr](auto req, auto) {
            if (req -> header().has_field("active_id")) {
                int activeId = std::stoi(req -> header().get_field("active_id"));

                return req->create_response().set_body(cp::serialize(actives::active(pool_ptr, activeId))).done();
            } else if (req -> header().has_field("active_ticker")) {
                std::string activeTicker = req -> header().get_field("active_ticker");

                return req->create_response().set_body(cp::serialize(actives::active(pool_ptr, activeTicker))).done();
            } else {
                return req->create_response(restinio::status_non_authoritative_information()).done();
            }
        });
    }

    void history(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
        router.get()->http_get("/actives/history", [pool_ptr, logger_ptr](auto req, auto) {
            if (req -> header().has_field("active_id")) {
                int activeId = std::stoi(req -> header().get_field("active_id"));

                return req->create_response().set_body(cp::serialize(actives::active_history(pool_ptr, activeId))).done();
            } else if (req -> header().has_field("active_ticker")) {
                std::string activeTicker = req -> header().get_field("active_ticker");

                return req->create_response().set_body(cp::serialize(actives::active_history(pool_ptr, activeTicker))).done();
            } else if (req -> header().has_field("user_id")) {
                int userId = std::stoi(req -> header().get_field("user_id"));

                return req->create_response().set_body(cp::serialize(actives::user_history(pool_ptr, userId))).done();
            } else {
                return req->create_response(restinio::status_non_authoritative_information()).done();
            }
        });
    }
}