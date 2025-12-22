#include "foundation/logger.h"
#include <spdlog/spdlog.h>

namespace foundation {
    void init_logger() {
        spdlog::set_level(spdlog::level::info);
        spdlog::info("Logger initialized");
    }
}
