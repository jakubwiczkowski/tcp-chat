#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <string>


class logger {
public:
    enum class level { INFO, WARNING, ERROR };

    explicit logger() = default;

    void log(level level, const std::string& message);

private:
    std::mutex logger_mutex;

    [[nodiscard]] std::string get_timestamp() const;
    [[nodiscard]] std::string get_thread_id() const;

    static std::string level_to_string(const level level) {
        switch (level) {
            case level::INFO:
                return "INFO";
            case level::WARNING:
                return "WARNING";
            case level::ERROR:
                return "ERROR";
            default:
                return "UNKNOWN";
        }
    }
};


#endif //LOGGER_H