#include <iostream>
#include <fstream>
#include <memory>
#include <string>

enum class Type {
    Warning,
    Error,
    FatalError,
    Unknown
};

class LogMessage {
    Type type_;
    std::string msg_;
public:
    LogMessage(Type type, std::string msg)
        : type_(type), msg_(std::move(msg)) {
    }

    Type type() const { return type_; }
    const std::string& message() const { return msg_; }
};

class LogHandler {
protected:
    std::shared_ptr<LogHandler> next_;
public:
    virtual ~LogHandler() = default;

    void setNext(std::shared_ptr<LogHandler> next) {
        next_ = next;
    }

    virtual void handle(const LogMessage& msg) {
        if (next_) next_->handle(msg);
        else throw std::runtime_error("Unhandled log message: " + msg.message());
    }
};

class WarningHandler : public LogHandler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::Warning) {
            std::cout << "[Warning] " << msg.message() << std::endl;
        }
        else if (next_) {
            next_->handle(msg);
        }
    }
};

class ErrorHandler : public LogHandler {
    std::string path_;
public:
    explicit ErrorHandler(std::string path) : path_(std::move(path)) {}
    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::Error) {
            std::ofstream out(path_, std::ios::app);
            if (out.is_open()) {
                out << "[Error] " << msg.message() << std::endl;
            }
        }
        else if (next_) {
            next_->handle(msg);
        }
    }
};

class FatalHandler : public LogHandler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::FatalError) {
            throw std::runtime_error("[FATAL] " + msg.message());
        }
        else if (next_) {
            next_->handle(msg);
        }
    }
};

class UnknownHandler : public LogHandler {
public:
    void handle(const LogMessage& msg) override {
        if (msg.type() == Type::Unknown) {
            throw std::runtime_error("Unknown message: " + msg.message());
        }
        else if (next_) {
            next_->handle(msg);
        }
    }
};

int main() {
    auto fatal = std::make_shared<FatalHandler>();
    auto error = std::make_shared<ErrorHandler>("errors.log");
    auto warning = std::make_shared<WarningHandler>();
    auto unknown = std::make_shared<UnknownHandler>();

    fatal->setNext(error);
    error->setNext(warning);
    warning->setNext(unknown);

    try {
        fatal->handle(LogMessage(Type::Warning, "Low battery"));
        fatal->handle(LogMessage(Type::Error, "Disk full"));
        fatal->handle(LogMessage(Type::FatalError, "Kernel panic"));
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}
