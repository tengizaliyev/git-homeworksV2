#include <iostream>
#include <fstream>
#include <string>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << "[Console] " << message << std::endl;
    }
};

class FileLogCommand : public LogCommand {
private:
    std::string path_;
public:
    explicit FileLogCommand(std::string path) : path_(std::move(path)) {}

    void print(const std::string& message) override {
        std::ofstream out(path_, std::ios::app);
        if (out.is_open()) {
            out << "[File] " << message << std::endl;
        }
    }
};

void print(LogCommand& command, const std::string& message) {
    command.print(message);
}

int main() {
    ConsoleLogCommand console;
    FileLogCommand file("log.txt");

    print(console, "Hello, console!");
    print(file, "Hello, file!");
}
