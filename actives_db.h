#include "../pqxx_cp.h"
#include <pqxx/pqxx>

pqxx::result all_public(std::shared_ptr<cp::connection_pool> pool_ptr);

pqxx::result active(std::shared_ptr<cp::connection_pool> pool_ptr, int activeId);

pqxx::result active(std::shared_ptr<cp::connection_pool> pool_ptr, std::string activeTicker);

pqxx::result active_history(std::shared_ptr<cp::connection_pool> pool_ptr, int activeId);

pqxx::result active_history(std::shared_ptr<cp::connection_pool> pool_ptr, std::string activeTicker);

pqxx::result user_actives(std::shared_ptr<cp::connection_pool> pool_ptr, int userId);

pqxx::result user_actives(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, int activeId);

pqxx::result user_actives(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, std::string activeTicker);

pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId);

pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, int activeId);

pqxx::result user_history(std::shared_ptr<cp::connection_pool> pool_ptr, int userId, std::string activeTicker);