#include "DOM.h"

namespace actives::deals {
    bool DOM::add_bid(int bid_id, std::string user_name, int price, int amount, actives::active_obj act) {
        bids[price].push(bid(price, bid_id, user_name, act, amount));
        return true;
    }

    std::vector<bid> DOM::check_bids() {
        auto start = bids.begin();
        auto end = bids.end();
        std::vector<bid> res;

        while (start != end && start -> first < 0 && end -> first > 0) {
            if (start -> first * (-1) < end -> first) {
                ++start;
            } else if (start -> first * (-1) > end -> first) {
                --end;
            } else if (start -> first * (-1) == end -> first) {
                while(!start -> second.empty() && !end -> second.empty()) {
                    auto b1 = start -> second.top();
                    auto b2 = end -> second.top();
                    if (b1->bidVal.price == b2->bidVal.price) {
                        int ammount = std::min(b1->bidVal.amount, b2->bidVal.amount);
                        res.push_back(bid(b1->bidVal.price, b1->bidVal.bid_id, b1->bidVal.owner, b1->bidVal.active, ammount, true));
                        b1->bidVal.amount -= ammount;
                        res.push_back(bid(b2->bidVal.price, b2->bidVal.bid_id, b2->bidVal.owner, b2->bidVal.active, ammount, true));
                        b2->bidVal.amount -= ammount;
                        if (b1->bidVal.amount == 0) {
                            start -> second.pop();
                        }
                        if (b2->bidVal.amount == 0) {
                            end -> second.pop();
                        }
                    }
                }
            }
            
        }
        return res;
    }
    
    void DOM::remove_bid(int price, int bid_id) {
        if (bids.find(price) == bids.end()) {
            return;
        }
        bids[price].delBid(bid_id);
    }

    void DOM::remove_bid(bid b) {
        remove_bid(b.price, b.bid_id);
    }
}