#pragma once
#include <string>
#include <vector>
#include <map>
#include <sstream>


class SqlSelectQueryBuilder {
protected:
    std::vector<std::string> m_columns;
    std::string m_table;
    std::vector<std::string> m_where;

public:
    virtual SqlSelectQueryBuilder& AddColumn(const std::string& column) noexcept {
        m_columns.push_back(column);
        return *this;
    }

    virtual SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept {
        for (const auto& col : columns) {
            m_columns.push_back(col);
        }
        return *this;
    }

    virtual SqlSelectQueryBuilder& AddFrom(const std::string& table) noexcept {
        m_table = table;
        return *this;
    }

    virtual SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value) noexcept {
        m_where.push_back(key + "=" + value);
        return *this;
    }

    virtual SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        for (const auto& [key, value] : kv) {
            m_where.push_back(key + "=" + value);
        }
        return *this;
    }

    virtual std::string BuildQuery() const {
        std::ostringstream query;

        query << "SELECT ";
        if (m_columns.empty()) {
            query << "*";
        }
        else {
            for (size_t i = 0; i < m_columns.size(); ++i) {
                query << m_columns[i];
                if (i < m_columns.size() - 1)
                    query << ", ";
            }
        }

        if (!m_table.empty())
            query << " FROM " << m_table;

        if (!m_where.empty()) {
            query << " WHERE ";
            for (size_t i = 0; i < m_where.size(); ++i) {
                query << m_where[i];
                if (i < m_where.size() - 1)
                    query << " AND ";
            }
        }

        query << ";";
        return query.str();
    }

    virtual ~SqlSelectQueryBuilder() = default;
};



class AdvancedSqlSelectQueryBuilder : public SqlSelectQueryBuilder {
public:
    using SqlSelectQueryBuilder::AddWhere;

    AdvancedSqlSelectQueryBuilder& AddWhere(const std::string& key,
        const std::string& op,
        const std::string& value) noexcept {
        m_where.push_back(key + op + value);
        return *this;
    }

    AdvancedSqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept override {
        SqlSelectQueryBuilder::AddColumns(columns);
        return *this;
    }

    AdvancedSqlSelectQueryBuilder& AddFrom(const std::string& table) noexcept override {
        SqlSelectQueryBuilder::AddFrom(table);
        return *this;
    }
};
