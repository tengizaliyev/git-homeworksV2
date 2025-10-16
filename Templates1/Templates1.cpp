#include <iostream>
#include "sql_query_builder.h"

int main() {
    SqlSelectQueryBuilder query_builder;

    query_builder
        .AddColumn("name")
        .AddColumn("phone")
        .AddFrom("students")
        .AddWhere("id", "42")
        .AddWhere("name", "John");

    std::cout << "[OUT]:" << std::endl;
    std::cout << query_builder.BuildQuery() << std::endl;

    return 0;
}
