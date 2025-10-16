#include <iostream>
#include <fstream>
#include <string>
#include <memory>

class TextPrintable {
public:
    virtual ~TextPrintable() = default;
    virtual std::string toText() const = 0;
};

class HTMLPrintable {
public:
    virtual ~HTMLPrintable() = default;
    virtual std::string toHTML() const = 0;
};

class JSONPrintable {
public:
    virtual ~JSONPrintable() = default;
    virtual std::string toJSON() const = 0;
};

class TextData : public TextPrintable {
private:
    std::string data_;
public:
    explicit TextData(std::string data) : data_(std::move(data)) {}
    std::string toText() const override {
        return data_;
    }
};

class HTMLData : public HTMLPrintable {
private:
    std::string data_;
public:
    explicit HTMLData(std::string data) : data_(std::move(data)) {}
    std::string toHTML() const override {
        return "<html>" + data_ + "</html>";
    }
};

class JSONData : public JSONPrintable {
private:
    std::string data_;
public:
    explicit JSONData(std::string data) : data_(std::move(data)) {}
    std::string toJSON() const override {
        return "{ \"data\": \"" + data_ + "\" }";
    }
};

void saveToAsText(std::ofstream& file, const TextPrintable& printable) {
    file << printable.toText();
}

void saveToAsHTML(std::ofstream& file, const HTMLPrintable& printable) {
    file << printable.toHTML();
}

void saveToAsJSON(std::ofstream& file, const JSONPrintable& printable) {
    file << printable.toJSON();
}

int main() {
    std::ofstream text_file("text.txt");
    std::ofstream html_file("page.html");
    std::ofstream json_file("data.json");

    TextData text("Hello Text!");
    HTMLData html("Hello HTML!");
    JSONData json("Hello JSON!");

    saveToAsText(text_file, text);
    saveToAsHTML(html_file, html);
    saveToAsJSON(json_file, json);

    std::cout << "All data saved successfully.\n";
}
