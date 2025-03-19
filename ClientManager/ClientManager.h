#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include <string>
#include <vector>
#include <pqxx/pqxx>

struct Client {
    int id;
    std::string first_name;
    std::string last_name;
    std::string email;
    std::vector<std::string> phones;
};

class ClientManager {
private:
    pqxx::connection conn;

    bool clientExists(int client_id);

public:
    ClientManager(const std::string& conninfo);
    ~ClientManager();

    void createTables();
    void addClient(const std::string& first_name, const std::string& last_name, const std::string& email);
    void addPhone(int client_id, const std::string& phone_number);
    void updateClient(int client_id, const std::string& first_name, const std::string& last_name, const std::string& email);
    void deletePhone(int client_id, const std::string& phone_number);
    void deleteClient(int client_id);
    std::vector<Client> findClients(const std::string& search_term);
    std::vector<Client> getAllClients();
};

#endif