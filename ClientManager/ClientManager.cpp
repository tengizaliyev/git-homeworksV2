#include "ClientManager.h"
#include <iostream>

ClientManager::ClientManager(const std::string& conninfo) : conn(conninfo) {
    if (conn.is_open()) {
        std::cout << "Подключение к базе данных успешно!" << std::endl;
    }
    else {
        throw std::runtime_error("Не удалось подключиться к базе данных");
    }
}

ClientManager::~ClientManager() {
    conn.close();
}

void ClientManager::createTables() {
    try {
        pqxx::work tx{ conn };
        tx.exec(
            "CREATE TABLE IF NOT EXISTS Clients ("
            "client_id SERIAL PRIMARY KEY,"
            "first_name VARCHAR(100) NOT NULL,"
            "last_name VARCHAR(100) NOT NULL,"
            "email VARCHAR(100) NOT NULL UNIQUE"
            ");"
            "CREATE TABLE IF NOT EXISTS Phones ("
            "phone_id SERIAL PRIMARY KEY,"
            "client_id INT NOT NULL,"
            "phone_number VARCHAR(20) NOT NULL,"
            "FOREIGN KEY (client_id) REFERENCES Clients(client_id) ON DELETE CASCADE"
            ");"
        );
        tx.commit();
        std::cout << "Таблицы успешно созданы!" << std::endl;
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "Ошибка создания таблиц: " << e.what() << std::endl;
    }
}

void ClientManager::addClient(const std::string& first_name, const std::string& last_name, const std::string& email) {
    try {
        pqxx::work tx{ conn };
        tx.exec_params(
            "INSERT INTO Clients (first_name, last_name, email) VALUES ($1, $2, $3);",
            first_name, last_name, email
        );
        tx.commit();
        std::cout << "Клиент добавлен: " << first_name << " " << last_name << std::endl;
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "Ошибка добавления клиента: " << e.what() << std::endl;
    }
}

bool ClientManager::clientExists(int client_id) {
    try {
        pqxx::work tx{ conn };
        auto result = tx.exec_params("SELECT 1 FROM Clients WHERE client_id = $1;", client_id);
        return !result.empty();
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "Ошибка проверки клиента: " << e.what() << std::endl;
        return false;
    }
}

void ClientManager::addPhone(int client_id, const std::string& phone_number) {
    if (!clientExists(client_id)) {
        std::cerr << "Клиент с ID " << client_id << " не существует" << std::endl;
        return;
    }
    try {
        pqxx::work tx{ conn };
        tx.exec_params(
            "INSERT INTO Phones (client_id, phone_number) VALUES ($1, $2);",
            client_id, phone_number
        );
        tx.commit();
        std::cout << "Телефон добавлен для клиента ID " << client_id << std::endl;
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "Ошибка добавления телефона: " << e.what() << std::endl;
    }
}

void ClientManager::updateClient(int client_id, const std::string& first_name, const std::string& last_name, const std::string& email) {
    if (!clientExists(client_id)) {
        std::cerr << "Клиент с ID " << client_id << " не существует" << std::endl;
        return;
    }
    try {
        pqxx::work tx{ conn };
        tx.exec_params(
            "UPDATE Clients SET first_name = $1, last_name = $2, email = $3 WHERE client_id = $4;",
            first_name, last_name, email, client_id
        );
        tx.commit();
        std::cout << "Данные клиента ID " << client_id << " обновлены" << std::endl;
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "Ошибка обновления клиента: " << e.what() << std::endl;
    }
}

void ClientManager::deletePhone(int client_id, const std::string& phone_number) {
    if (!clientExists(client_id)) {
        std::cerr << "Клиент с ID " << client_id << " не существует" << std::endl;
        return;
    }
    try {
        pqxx::work tx{ conn };
        tx.exec_params(
            "DELETE FROM Phones WHERE client_id = $1 AND phone_number = $2;",
            client_id, phone_number
        );
        tx.commit();
        std::cout << "Телефон удалён для клиента ID " << client_id << std::endl;
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "Ошибка удаления телефона: " << e.what() << std::endl;
    }
}

void ClientManager::deleteClient(int client_id) {
    if (!clientExists(client_id)) {
        std::cerr << "Клиент с ID " << client_id << " не существует" << std::endl;
        return;
    }
    try {
        pqxx::work tx{ conn };
        tx.exec_params("DELETE FROM Clients WHERE client_id = $1;", client_id);
        tx.commit();
        std::cout << "Клиент ID " << client_id << " удалён" << std::endl;
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "Ошибка удаления клиента: " << e.what() << std::endl;
    }
}

std::vector<Client> ClientManager::findClients(const std::string& search_term) {
    std::vector<Client> result;
    try {
        pqxx::work tx{ conn };
        auto rows = tx.exec_params(
            "SELECT c.client_id, c.first_name, c.last_name, c.email, p.phone_number "
            "FROM Clients c LEFT JOIN Phones p ON c.client_id = p.client_id "
            "WHERE c.first_name ILIKE '%' || $1 || '%' "
            "OR c.last_name ILIKE '%' || $1 || '%' "
            "OR c.email ILIKE '%' || $1 || '%' "
            "OR p.phone_number ILIKE '%' || $1 || '%';",
            search_term
        );
        std::map<int, Client> clients_map;
        for (const auto& row : rows) {
            int client_id = row[0].as<int>();
            if (clients_map.find(client_id) == clients_map.end()) {
                Client client;
                client.id = client_id;
                client.first_name = row[1].as<std::string>();
                client.last_name = row[2].as<std::string>();
                client.email = row[3].as<std::string>();
                clients_map[client_id] = client;
            }
            if (!row[4].is_null()) {
                clients_map[client_id].phones.push_back(row[4].as<std::string>());
            }
        }
        for (const auto& pair : clients_map) {
            result.push_back(pair.second);
        }
    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "Ошибка поиска: " << e.what() << std::endl;
    }
    return result;
}

std::vector<Client> ClientManager::getAllClients() {
    return findClients("");
}