#include <stdio.h>
#include <queue>
#include <variant>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <iostream>
#include <unistd.h>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int A_PRIORITY = 1;
int B_PRIORITY = 2;

class Priority {
public:
    Priority(int priority) : _priority{priority} {
        // DO NOTHING
    }

    int get_priority() const {return _priority; }
private:
    int _priority;
};

struct APriority : public Priority {
    APriority() : Priority(A_PRIORITY) {
        // DO NOTHING
    }
};


struct BPriority : public Priority {
    BPriority() : Priority(B_PRIORITY) {
        // DO NOTHING
    }
};

using ABRequest = std::variant<APriority, BPriority>;

struct VisitOverload {
    int priority;
    void operator()(const APriority& a) {
        priority = a.get_priority();
    }
    void operator()(const BPriority& b) {
        priority = b.get_priority();
    }
};

struct ABRequestComparator {

    bool operator()(const ABRequest& lhs, const ABRequest& rhs) {
        VisitOverload lhs_visitor;
        VisitOverload rhs_visitor;

        std::visit(lhs_visitor, lhs);
        std::visit(rhs_visitor, rhs);

        return lhs_visitor.priority < rhs_visitor.priority;

        // auto get_level = [](const ABRequest& req, int&level) {
        //     std::visit(
        //         overload(
        //             [&](const APriority& a) {level = a.get_priority();},
        //             [&](const BPriority& b) {level = b.get_priority();}
        //         ),
        //         req;
        //     );
        // };

        // int lhs_priority = 0;
        // get_level(lhs, lhs_priority);
        // int rhs_priority = 0;
        // get_level(rhs, rhs_priority);

        // return lhs_level < rhs_level;
    }
};

class ABRequestQueue {
public:
    ABRequestQueue() = default;

    ABRequest get_next_request() {
        std::unique_lock lock{_queue_lock};

        _data_cond.wait(lock, [this]() { return !_priority_queue.empty(); });

        ABRequest req = _priority_queue.top();
        _priority_queue.pop();
        return req;
    }

    void push_request(const ABRequest& req) {
        std::scoped_lock lock{_queue_lock};
        _priority_queue.push(req);
        _data_cond.notify_one();
    }

    bool is_empty() {
        std::scoped_lock lock{_queue_lock};
        return _priority_queue.empty();
    }

private:
    std::mutex _queue_lock;
    std::condition_variable _data_cond;
    std::priority_queue<ABRequest, std::vector<ABRequest>, ABRequestComparator> _priority_queue;
};


static ABRequestQueue g_req_queue;

static void producer_thread() {
    for (int i = 0; i < 100; i++) {
        usleep(400 * 1000);
        ABRequest req;
        if (i % 3 != 0) {
            // printf("pusing APriority\n");
            req = APriority{};
        } else {
            // printf("pusing BPriority\n");
            req = BPriority{};
        }

        g_req_queue.push_request(req);
    }
}

static void consumer_thread() {
    while (true) {
        sleep(1);
        while(!g_req_queue.is_empty()) {
            ABRequest req = g_req_queue.get_next_request();
            std::visit(
                overloaded(
                    [](APriority a) { printf("Got APriority\n"); },
                    [](BPriority b) { printf("Got BPriority\n"); }
                ),
                req
            );
        }
        printf("===============================================\n");
    }
}



int main() {
    std::thread p_thread{producer_thread};
    std::thread c_thread{consumer_thread};

    p_thread.join();
    c_thread.join();
    return 0;
}