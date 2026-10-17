#pragma once

struct FlushStats {
    double wait_for_async_io_seconds{0.0};
    double sync_seconds{0.0};
};
