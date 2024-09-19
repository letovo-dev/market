#include "actives_db.h"

pqxx::result all_public(std::shared_ptr<cp::connection_pool> pool_ptr) {
    cp::query get_all_public("SELECT * FROM \"active\" WHERE \"ispublic\";");

    auto tx = cp::tx(*pool_ptr, get_all_public);

    pqxx::result result = get_all_public();

    return result;
}


pqxx::result active(std::shared_ptr<cp::connection_pool> pool_ptr, int activeId) {
    cp::query get_active("SELECT * FROM \"active\" WHERE \"ispublic\" AND activeId = ($1);");

    auto tx = cp::tx(*pool_ptr, get_active);

    pqxx::result result = get_active(activeId);

    return result;
}

pqxx::result active(std::shared_ptr<cp::connection_pool> pool_ptr, std::string activeTicker) {
    cp::query get_active("SELECT * FROM \"active\" WHERE \"ispublic\" AND activeTicker = ($1);");

    auto tx = cp::tx(*pool_ptr, get_active);

    pqxx::result result = get_active(activeTicker);

    return result;
}

pqxx::result active_history(std::shared_ptr<cp::connection_pool> pool_ptr, int activeId) {
    cp::query get_active("SELECT ah.* FROM \"activeHistory\" ah JOIN \"active\" a ON ah.activeId = a.activeId WHERE a.ispublic = true AND ah.activeId = ($1);");

    auto tx = cp::tx(*pool_ptr, get_active);

    pqxx::result result = get_active(activeId);

    return result;
}

pqxx::result active_history(std::shared_ptr<cp::connection_pool> pool_ptr, std::string activeTicker) {
    cp::query get_active("SELECT ah.* FROM \"activeHistory\" ah JOIN \"active\" a ON ah.activeId = a.activeId WHERE a.ispublic = true AND a.activeTicker = ($1);");

    auto tx = cp::tx(*pool_ptr, get_active);

    pqxx::result result = get_active(activeTicker);

    return result;
}

pqxx::result user_actives(std::shared_ptr<cp::connection_pool> pool_ptr, int userId) {

}

pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId) {
    cp::query get_active("SELECT * FROM \"activeHistory\" WHERE userId=($1);");

    auto tx = cp::tx(*pool_ptr, get_active);

    pqxx::result result = get_active(userId);

    return result;
}

pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, int activeId) {
    cp::query get_active("SELECT * FROM \"activeHistory\" WHERE userId=($1) AND activeId = ($2);");

    auto tx = cp::tx(*pool_ptr, get_active);

    pqxx::result result = get_active(userId, activeId);

    return result;
}

pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, std::string activeTicker) {     
    cp::query get_active("SELECT ah.* FROM \"activeHistory\" ah JOIN \"active\" a ON ah.activeId = a.activeId WHERE ah.userId=($1) AND a.activeTicker = ($2);");

    auto tx = cp::tx(*pool_ptr, get_active);

    pqxx::result result = get_active(userId, activeTicker);

    return result;
}