#pragma once

#include "executor/request_id.hpp"

struct Completion {
    RequestId request_id;
    int result{0};
};
