#include <iostream>
#include <pqxx/pqxx>
#include <vector>

using namespace std;
using namespace pqxx;

struct ClientInfo {
    int id;
    string first_name;
    string last_name;
    string email;
    string phone;
};

class ClientManager {
private:
    connection* C;

public:
    ClientManager(const string& conn_str) {
        C = new connection(conn_str);
        if (C->is_open()) {
            cout << "Connected to database successfully!" << endl;
        }
        else {
            throw runtime_error("Can't open database");
        }
    }

    ~ClientManager() {
        delete C;  // Автоматически закроется
    }

    void createTables() {
        work W(*C);
        W.exec(R"(
            CREATE TABLE IF NOT EXISTS clients (
                id SERIAL PRIMARY KEY,
                first_name TEXT,
                last_name TEXT,
                email TEXT UNIQUE
            );
            CREATE TABLE IF NOT EXISTS phones (
                id SERIAL PRIMARY KEY,
                client_id INTEGER REFERENCES clients(id) ON DELETE CASCADE,
                phone TEXT
            );
        )");
        W.commit();
    }

    void addClient(const string& first_name, const string& last_name, const string& email) {
        work W(*C);
        W.exec(zview("INSERT INTO clients (first_name, last_name, email) VALUES ($1, $2, $3);"),
            std::make_tuple(first_name, last_name, email));
        W.commit();
    }

    void addPhone(int client_id, const string& phone) {
        work W(*C);
        W.exec(zview("INSERT INTO phones (client_id, phone) VALUES ($1, $2);"),
            std::make_tuple(client_id, phone));
        W.commit();
    }

    void updateClient(int client_id, const string& first_name, const string& last_name, const string& email) {
        work W(*C);
        W.exec(zview("UPDATE clients SET first_name=$1, last_name=$2, email=$3 WHERE id=$4;"),
            std::make_tuple(first_name, last_name, email, client_id));
        W.commit();
    }

    void deletePhone(int phone_id) {
        work W(*C);
        W.exec(zview("DELETE FROM phones WHERE id=$1;"),
            std::make_tuple(phone_id));
        W.commit();
    }

    void deleteClient(int client_id) {
        work W(*C);
        W.exec(zview("DELETE FROM clients WHERE id=$1;"),
            std::make_tuple(client_id));
        W.commit();
    }

    vector<ClientInfo> findClient(const string& search) {
        work W(*C);
        zview query = R"(
            SELECT c.id, first_name, last_name, email, phone
            FROM clients c
            LEFT JOIN phones p ON c.id = p.client_id
            WHERE first_name ILIKE $1 OR last_name ILIKE $1 OR email ILIKE $1 OR phone ILIKE $1;
        )";

        result R = W.exec(query, std::make_tuple("%" + search + "%"));

        vector<ClientInfo> results;
        for (const auto& row : R) {
            results.push_back(ClientInfo{
                row["id"].as<int>(),
                row["first_name"].as<string>(),
                row["last_name"].as<string>(),
                row["email"].as<string>(),
                row["phone"].is_null() ? "" : row["phone"].as<string>()
                });
        }

        return results;
    }
};

int main() {
    try {
        string conn_str = "host=localhost port=5432 dbname=postgres user=postgres password=Tengiz200303Aliyev";
        ClientManager manager(conn_str);

        manager.createTables();
        manager.addClient("Tony", "Stark", "ironman@avengers.com");
        manager.addPhone(1, "1234567890");
        manager.addPhone(1, "9876543210");
        manager.updateClient(1, "Anthony", "Stark", "tony@avengers.com");

        vector<ClientInfo> results = manager.findClient("tony");

        for (const auto& client : results) {
            cout << "ID: " << client.id
                << ", Name: " << client.first_name << " " << client.last_name
                << ", Email: " << client.email
                << ", Phone: " << (client.phone.empty() ? "None" : client.phone)
                << endl;
        }

        manager.deletePhone(1);
        manager.deleteClient(1);
    }
    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
