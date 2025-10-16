#include <iostream>
#include "sql_query_builder.h"

int main() {
    AdvancedSqlSelectQueryBuilder query_builder;

    query_builder
        .AddColumns({ "name", "phone" })
        .AddFrom("students")
        .AddWhere("id", ">", "42")
        .AddWhere("age", "<", "18")
        .AddWhere("status", "active");

    std::cout << "[OUT]:" << std::endl;
    std::cout << query_builder.BuildQuery() << std::endl;

    return 0;
}
