#include "../basic/pqxx_cp.h"
#include "../basic/utils.h"
#include "actives_db.h"
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>

namespace actives::deals {
    class DOM {
        private:
            // contains {price, bid}
            std::map<int, utils::LinkedList> bids;

        public:
            DOM(std::shared_ptr<cp::ConnectionsManager> pool_ptr);

            bool add_bid(int bid_id, std::string user_name, int price, int amount, actives::active_obj act);

            void remove_bid(int, int);

            void remove_bid(bid);

            std::vector<bid> check_bids();
    };
}