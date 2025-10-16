#pragma once
#include <string>
#include <vector>
#include <map>
#include <sstream>

class SqlSelectQueryBuilder {
private:
    std::vector<std::string> m_columns;
    std::string m_table;
    std::map<std::string, std::string> m_where;

public:
    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        m_columns.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) {
        m_table = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value) {
        m_where[key] = value;
        return *this;
    }

    std::string BuildQuery() const {
        std::ostringstream query;

        query << "SELECT ";
        if (m_columns.empty()) {
            query << "*";
        }
        else {
            for (size_t i = 0; i < m_columns.size(); ++i) {
                query << m_columns[i];
                if (i < m_columns.size() - 1) query << ", ";
            }
        }

        if (!m_table.empty()) {
            query << " FROM " << m_table;
        }

       
        if (!m_where.empty()) {
            query << " WHERE ";
            size_t i = 0;
            for (const auto& [key, value] : m_where) {
                query << key << "=" << value;
                if (i < m_where.size() - 1) query << " AND ";
                ++i;
            }
        }

        query << ";";
        return query.str();
    }
};
