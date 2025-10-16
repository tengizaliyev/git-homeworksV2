#include <iostream>
#include "sql_query_builder.h"

int main() {
    SqlSelectQueryBuilder query_builder;

    query_builder.AddColumns({ "id", "name", "phone" });

    query_builder.AddFrom("students");

    query_builder.AddWhere({ {"id", "42"}, {"name", "John"}, {"group", "A1"} });

    std::cout << "[OUT]:" << std::endl;
    std::cout << query_builder.BuildQuery() << std::endl;

    return 0;
}
