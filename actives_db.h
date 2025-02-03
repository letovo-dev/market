#pragma once

#include "../basic/pqxx_cp.h"
#include <pqxx/pqxx>
#include <restinio/all.hpp>
#include "../basic/url_parser.h"
#include "../basic/auth.h"


namespace actives {
    struct active_obj {
        int activeId;
        std::string activeTicker;
    };

    pqxx::result all_public(std::shared_ptr<cp::ConnectionsManager> pool_ptr);

    pqxx::result active(std::shared_ptr<cp::ConnectionsManager> pool_ptr, actives::active_obj act);

    pqxx::result active_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr,  actives::active_obj act);

    pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId);

    pqxx::result user_actives(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId, actives::active_obj act);

    pqxx::result user_history(std::shared_ptr<cp::ConnectionsManager> pool_ptr, int userId);
}

namespace actives::server {
    void all_public(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

    void active(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

    void history(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);
}

namespace actives::deals {
    bool deal_resolver();
    
    void add_bid(std::unique_ptr<restinio::router::express_router_t<>>& router, std::shared_ptr<cp::ConnectionsManager> pool_ptr, std::shared_ptr<restinio::shared_ostream_logger_t> logger_ptr);

    pqxx::result DOM(int active_id);

    pqxx::result DOM(std::string active_ticket);
}