#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class ObservableLogger {
private:
    std::vector<Observer*> observers_;
public:
    void addObserver(Observer* obs) {
        observers_.push_back(obs);
    }

    void removeObserver(Observer* obs) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), obs), observers_.end());
    }

    void warning(const std::string& message) const {
        for (auto* obs : observers_)
            if (obs) obs->onWarning(message);
    }

    void error(const std::string& message) const {
        for (auto* obs : observers_)
            if (obs) obs->onError(message);
    }

    void fatalError(const std::string& message) const {
        for (auto* obs : observers_)
            if (obs) obs->onFatalError(message);
    }
};


class WarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "[Warning] " << message << std::endl;
    }
};

class ErrorObserver : public Observer {
private:
    std::string path_;
public:
    explicit ErrorObserver(std::string path) : path_(std::move(path)) {}

    void onError(const std::string& message) override {
        std::ofstream out(path_, std::ios::app);
        if (out.is_open()) {
            out << "[Error] " << message << std::endl;
        }
    }
};

class FatalErrorObserver : public Observer {
private:
    std::string path_;
public:
    explicit FatalErrorObserver(std::string path) : path_(std::move(path)) {}

    void onFatalError(const std::string& message) override {
        std::cout << "[FATAL] " << message << std::endl;
        std::ofstream out(path_, std::ios::app);
        if (out.is_open()) {
            out << "[FATAL] " << message << std::endl;
        }
    }
};

int main() {
    ObservableLogger logger;

    WarningObserver warnObs;
    ErrorObserver errObs("errors.log");
    FatalErrorObserver fatalObs("fatal.log");

    logger.addObserver(&warnObs);
    logger.addObserver(&errObs);
    logger.addObserver(&fatalObs);

    logger.warning("Low memory");
    logger.error("File not found");
    logger.fatalError("System crash");
}
