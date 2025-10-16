#include <map>
#include <string>
#include <iostream>

class VeryHeavyDatabase {
public:
    std::string GetData(const std::string& key) const noexcept {
        return "Very Big Data String: " + key;
    }
};

class CacheProxyDB : public VeryHeavyDatabase {
public:
    explicit CacheProxyDB(VeryHeavyDatabase* real_object) : real_db_(real_object) {}

    std::string GetData(const std::string& key) noexcept {
        if (cache_.find(key) == cache_.end()) {
            std::cout << "Get from real object\n";
            cache_[key] = real_db_->GetData(key);
        }
        else {
            std::cout << "Get from cache\n";
        }
        return cache_.at(key);
    }

private:
    std::map<std::string, std::string> cache_;
    VeryHeavyDatabase* real_db_;
};

class OneShotDB : public VeryHeavyDatabase {
private:
    VeryHeavyDatabase* real_db_;
    size_t shots_left_;
public:
    explicit OneShotDB(VeryHeavyDatabase* real_object, size_t shots = 1)
        : real_db_(real_object), shots_left_(shots) {
    }

    std::string GetData(const std::string& key) noexcept {
        if (shots_left_ > 0) {
            --shots_left_;
            return real_db_->GetData(key);
        }
        return "error";
    }
};

int main() {
    auto real_db = VeryHeavyDatabase();
    auto cache_db = CacheProxyDB(std::addressof(real_db));
    auto limited_db = OneShotDB(std::addressof(real_db), 2);

    std::cout << "\n--- CacheProxyDB ---\n";
    std::cout << cache_db.GetData("key") << std::endl;
    std::cout << cache_db.GetData("key") << std::endl;

    std::cout << "\n--- OneShotDB ---\n";
    std::cout << limited_db.GetData("key") << std::endl;
    std::cout << limited_db.GetData("key") << std::endl;
    std::cout << limited_db.GetData("key") << std::endl;
}
