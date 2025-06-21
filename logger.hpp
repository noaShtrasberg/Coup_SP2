// noashalom5@gmail.com

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <vector>

class Logger {
private:
    std::vector<std::string> logs;
    Logger() {}

public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string& message) {
        logs.push_back(message);
    }

    const std::vector<std::string>& getLogs() const {
        return logs;
    }

    void clear() {
        logs.clear();
    }

    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;
};

#endif
