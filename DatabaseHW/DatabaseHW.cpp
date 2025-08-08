#include <iostream>
#include <pqxx/pqxx>
using namespace std;
using namespace pqxx;

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
            cerr << "Can't open database" << endl;
            exit(1);
        }
    }

    ~ClientManager() {
        delete C; // Соединение закроется автоматически
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
        cout << "Tables created successfully." << endl;
    }

    void addClient(const string& first_name, const string& last_name, const string& email) {
        work W(*C);
        zview query = zview("INSERT INTO clients (first_name, last_name, email) VALUES ($1, $2, $3);");
        W.exec(query, std::make_tuple(first_name, last_name, email));
        W.commit();
        cout << "Client added." << endl;
    }

    void addPhone(int client_id, const string& phone) {
        work W(*C);
        zview query = zview("INSERT INTO phones (client_id, phone) VALUES ($1, $2);");
        W.exec(query, std::make_tuple(client_id, phone));
        W.commit();
        cout << "Phone added." << endl;
    }

    void updateClient(int client_id, const string& first_name, const string& last_name, const string& email) {
        work W(*C);
        zview query = zview("UPDATE clients SET first_name=$1, last_name=$2, email=$3 WHERE id=$4;");
        W.exec(query, std::make_tuple(first_name, last_name, email, client_id));
        W.commit();
        cout << "Client updated." << endl;
    }

    void deletePhone(int phone_id) {
        work W(*C);
        zview query = zview("DELETE FROM phones WHERE id=$1;");
        W.exec(query, std::make_tuple(phone_id));
        W.commit();
        cout << "Phone deleted." << endl;
    }

    void deleteClient(int client_id) {
        work W(*C);
        zview query = zview("DELETE FROM clients WHERE id=$1;");
        W.exec(query, std::make_tuple(client_id));
        W.commit();
        cout << "Client deleted." << endl;
    }

    void findClient(const string& search) {
        work W(*C);
        zview query = zview(R"(
            SELECT c.id, first_name, last_name, email, phone
            FROM clients c
            LEFT JOIN phones p ON c.id = p.client_id
            WHERE first_name ILIKE $1 OR last_name ILIKE $1 OR email ILIKE $1 OR phone ILIKE $1;
        )");
        result R = W.exec(query, std::make_tuple("%" + search + "%"));

        for (const auto& row : R) {
            cout << "ID: " << row["id"].as<int>() << ", Name: "
                << row["first_name"].as<string>() << " " << row["last_name"].as<string>()
                << ", Email: " << row["email"].as<string>()
                << ", Phone: " << (row["phone"].is_null() ? "None" : row["phone"].as<string>()) << endl;
        }
    }
};

int main() {
    string conn_str = "host=localhost port=5432 dbname=postgres user=postgres password=Tengiz200303Aliyev";
    ClientManager manager(conn_str);

    manager.createTables();
    manager.addClient("Tony", "Stark", "ironman@avengers.com");
    manager.addPhone(1, "1234567890");
    manager.addPhone(1, "9876543210");
    manager.updateClient(1, "Anthony", "Stark", "tony@avengers.com");
    manager.findClient("tony");
    manager.deletePhone(1);
    manager.deleteClient(1);

    return 0;
}
