#include "logger.h"

#include <iomanip>
#include <iostream>

void logger::log(const level level, const std::string& message) {
    std::lock_guard lock{this->logger_mutex};
    std::cout << get_timestamp() << " [tid:" << get_thread_id() << "] ["
              << level_to_string(level) << "] " << message << std::endl;
}

std::string logger::get_timestamp() const {
    const auto now = std::chrono::system_clock::now();
    const auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string logger::get_thread_id() const {
    std::stringstream ss;
    ss << std::this_thread::get_id();
    return ss.str();
}