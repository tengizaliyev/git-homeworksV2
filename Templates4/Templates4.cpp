#include <string>
#include <iostream>
#include <algorithm>

class Text {
public:
    virtual void render(const std::string& data) const {
        std::cout << data;
    }
    virtual ~Text() = default;
};

class DecoratedText : public Text {
protected:
    Text* text_;
public:
    explicit DecoratedText(Text* text) : text_(text) {}
};

class ItalicText : public DecoratedText {
public:
    using DecoratedText::DecoratedText;
    void render(const std::string& data) const override {
        std::cout << "<i>";
        text_->render(data);
        std::cout << "</i>";
    }
};

class BoldText : public DecoratedText {
public:
    using DecoratedText::DecoratedText;
    void render(const std::string& data) const override {
        std::cout << "<b>";
        text_->render(data);
        std::cout << "</b>";
    }
};

class Paragraph : public DecoratedText {
public:
    using DecoratedText::DecoratedText;
    void render(const std::string& data) const override {
        std::cout << "<p>";
        text_->render(data);
        std::cout << "</p>";
    }
};

class Reversed : public DecoratedText {
public:
    using DecoratedText::DecoratedText;
    void render(const std::string& data) const override {
        std::string reversed = data;
        std::reverse(reversed.begin(), reversed.end());
        text_->render(reversed);
    }
};

class Link : public DecoratedText {
public:
    using DecoratedText::DecoratedText;
    void render(const std::string& href, const std::string& text) const {
        std::cout << "<a href=" << href << ">";
        text_->render(text);
        std::cout << "</a>";
    }
};

int main() {
    std::cout << "Paragraph:\n";
    auto p = new Paragraph(new Text());
    p->render("Hello world");
    std::cout << "\n\n";

    std::cout << "Reversed:\n";
    auto r = new Reversed(new Text());
    r->render("Hello world");
    std::cout << "\n\n";

    std::cout << "Link:\n";
    auto l = new Link(new Text());
    l->render("netology.ru", "Hello world");
    std::cout << "\n";
}
