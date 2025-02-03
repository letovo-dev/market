#include "actives.h"


namespace actives {
    pqxx::result all_public(std::shared_ptr<cp::ConnectionsManager> pool_ptr) {
        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute("SELECT * FROM \"active\" WHERE \"ispublic\";");

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result active(std::shared_ptr<cp::ConnectionsManager> pool_ptr, active_obj act) {
        if(act.activeId != 0) {
            std::vector<int> params = {act.activeId};

            auto con = std::move(pool_ptr->getConnection());

            pqxx::result result = con->execute_params("SELECT * FROM \"active\" WHERE \"ispublic\" AND activeId = ($1);", params);

            pool_ptr->returnConnection(std::move(con));

            return result;
        } else if(act.activeTicker != "") {
            std::vector<std::string> params = {act.activeTicker};

            auto con = std::move(pool_ptr->getConnection());

            pqxx::result result = con->execute_params("SELECT * FROM \"active\" WHERE \"ispublic\" AND activeTicker = ($1);", params);

            pool_ptr->returnConnection(std::move(con));

            return result;
        }
        return pqxx::result();
    }

    pqxx::result active_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, active_obj act) {
        if(act.activeId != 0) {
            std::vector<int> params = {act.activeId};

            auto con = std::move(pool_ptr->getConnection());

            pqxx::result result = con->execute_params("SELECT ah.* FROM \"activeHistory\" ah JOIN \"active\" a ON ah.activeId = a.activeId WHERE a.ispublic = true AND ah.activeId = ($1);", params);

            pool_ptr->returnConnection(std::move(con));

            return result;
        } else if(act.activeTicker != "")  {
            std::vector<std::string> params = {act.activeTicker};

            auto con = std::move(pool_ptr->getConnection());

            pqxx::result result = con->execute_params("SELECT ah.* FROM \"activeHistory\" ah JOIN \"active\" a ON ah.activeId = a.activeId WHERE a.ispublic = true AND a.activeTicker = ($1);", params);

            pool_ptr->returnConnection(std::move(con));

            return result;
        }
        return pqxx::result();
    }

    pqxx::result active_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string activeTicker) {
        std::vector<std::string> params = {activeTicker};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT ah.* FROM \"activeHistory\" ah JOIN \"active\" a ON ah.activeId = a.activeId WHERE a.ispublic = true AND a.activeTicker = ($1);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string user_name) {
        std::vector<std::string> params = {user_name};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT * FROM usersactives WHERE user_name=($1);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string user_name, active_obj act) {
        if(act.activeId != 0) {
            std::vector<std::string> params = {user_name, std::to_string(act.activeId)};

            auto con = std::move(pool_ptr->getConnection());

            pqxx::result result = con->execute_params("SELECT * FROM usersactives WHERE user_name=($1) and activeId=($2);", params);

            pool_ptr->returnConnection(std::move(con));

            return result;
        } else if(act.activeTicker != "") {
            std::vector<std::string> params = {user_name, act.activeTicker};

            auto con = std::move(pool_ptr->getConnection());

            pqxx::result result = con->execute_params("SELECT ua.* FROM usersactives ua JOIN \"active\" a ON ua.activeId = a.activeId WHERE ua.user_name=($1) and a.activeTicker=($2);", params);

            pool_ptr->returnConnection(std::move(con));

            return result;
        }
        return pqxx::result();
    }

    pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string user_name) {
        std::vector<std::string> params = {user_name};

        auto con = std::move(pool_ptr->getConnection());

        pqxx::result result = con->execute_params("SELECT * FROM \"activeHistory\" WHERE user_name=($1);", params);

        pool_ptr->returnConnection(std::move(con));

        return result;
    }

    pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::string user_name, active_obj act) {
        if(act.activeId != 0) {
            std::vector<std::string> params = {user_name, std::to_string(act.activeId)};

            auto con = std::move(pool_ptr->getConnection());

            pqxx::result result = con->execute_params("SELECT * FROM \"activeHistory\" WHERE user_name=($1) and activeId=($2);", params);

            pool_ptr->returnConnection(std::move(con));

            return result;
        } else if(act.activeTicker != "") {
            std::vector<std::string> params = {user_name, act.activeTicker};

            auto con = std::move(pool_ptr->getConnection());

            pqxx::result result = con->execute_params("SELECT ah.* FROM \"activeHistory\" ah JOIN \"active\" a ON ah.activeId = a.activeId WHERE ah.user_name=($1) and a.activeTicker=($2);", params);

            pool_ptr->returnConnection(std::move(con));

            return result;
        }
        return pqxx::result();
    }
    
    void add_active(std::shared_ptr<cp::ConnectionsManager> pool_ptr, const std::string user_name, active_obj act) {
        std::vector<std::string> params = {user_name, act.activeTicker};

        auto con = std::move(pool_ptr->getConnection());

        con->execute_params("INSERT INTO usersactives (user_name, activeId) SELECT ($1), activeId FROM \"active\" WHERE activeTicker = ($2);", params);

        pool_ptr->returnConnection(std::move(con));
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
            active_obj act;
            if (req -> header().has_field("active_id")) {
                act.activeId = std::stoi(req -> header().get_field("active_id"));
            } else if (req -> header().has_field("active_ticker")) {
                act.activeTicker = req -> header().get_field("active_ticker");
            } else {
                return req->create_response(restinio::status_non_authoritative_information()).done();
            }
            return req->create_response().set_body(cp::serialize(actives::active(pool_ptr, act))).done();
        });
    }

    void history(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
        router.get()->http_get("/actives/history", [pool_ptr, logger_ptr](auto req, auto) {
            active_obj act;

            if (req -> header().has_field("active_id")) {
                act.activeId = std::stoi(req -> header().get_field("active_id"));
            } else if (req -> header().has_field("active_ticker")) {
                act.activeTicker = req -> header().get_field("active_ticker");
            } else {
                return req->create_response(restinio::status_non_authoritative_information()).done();
            }

            return req->create_response().set_body(cp::serialize(actives::active_history(pool_ptr, act))).done();
        });
    }

    void user_actives(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
        router.get()->http_get("/actives/user_actives", [pool_ptr, logger_ptr](auto req, auto) {
            std::string user_name;
            try {
                user_name = req -> header().get_field("user_name");
            } catch (const std::exception& e) {
                return req->create_response(restinio::status_non_authoritative_information()).done();
            }
            return req->create_response().set_body(cp::serialize(actives::user_actives(pool_ptr, user_name))).done();
        });
    }

    void user_history(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr) {
        router.get()->http_get("/actives/user_history", [pool_ptr, logger_ptr](auto req, auto) {
            std::string user_name;
            try {
                user_name = req -> header().get_field("user_name");
            } catch (const std::exception& e) {
                return req->create_response(restinio::status_non_authoritative_information()).done();
            }
            return req->create_response().set_body(cp::serialize(actives::user_history(pool_ptr, user_name))).done();
        });
    }
}