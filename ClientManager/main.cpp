#include "ClientManager.h"
#include <iostream>
#include <windows.h>

int main() {
    std::string conninfo = "host=localhost port=5432 dbname=clients_db user=postgres password=Tengiz200303Aliyev";
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    try {
        ClientManager manager(conninfo);

        // 1. Создаём таблицы
        manager.createTables();

        // 2. Добавляем клиентов
        manager.addClient("Sasha", "Ivanov", "ivan@example.com");
        manager.addClient("Kostya", "Petrov", "petr@example.com");

        // 3. Добавляем телефоны
        manager.addPhone(1, "+79991234567");
        manager.addPhone(1, "+79997654321");
        manager.addPhone(2, "+79991112233");

        // 4. Выводим всех клиентов
        std::cout << "\nAll clients:\n";
        auto allClients = manager.getAllClients();
        for (const auto& client : allClients) {
            std::cout << "ID: " << client.id << ", Name: " << client.first_name
                << ", Surname: " << client.last_name << ", Email: " << client.email
                << ", Phones: ";
            for (const auto& phone : client.phones) {
                std::cout << phone << " ";
            }
            std::cout << std::endl;
        }

        // 5. Ищем клиентов по имени
        std::cout << "\nSearch for 'Ivan':\n";
        auto clients = manager.findClients("Ivan");
        for (const auto& client : clients) {
            std::cout << "ID: " << client.id << ", Name: " << client.first_name
                << ", Surname: " << client.last_name << ", Email: " << client.email
                << ", Phones: ";
            for (const auto& phone : client.phones) {
                std::cout << phone << " ";
            }
            std::cout << std::endl;
        }

        // 6. Ищем клиентов по телефону
        std::cout << "\nSearch for phone '+79991234567':\n";
        clients = manager.findClients("+79991234567");
        for (const auto& client : clients) {
            std::cout << "ID: " << client.id << ", Name: " << client.first_name
                << ", Surname: " << client.last_name << ", Email: " << client.email
                << ", Phones: ";
            for (const auto& phone : client.phones) {
                std::cout << phone << " ";
            }
            std::cout << std::endl;
        }

        // 7. Ищем клиентов по email
        std::cout << "\nSearch for email 'petr@example.com':\n";
        clients = manager.findClients("petr@example.com");
        for (const auto& client : clients) {
            std::cout << "ID: " << client.id << ", Name: " << client.first_name
                << ", Surname: " << client.last_name << ", Email: " << client.email
                << ", Phones: ";
            for (const auto& phone : client.phones) {
                std::cout << phone << " ";
            }
            std::cout << std::endl;
        }

        // 8. Обновляем данные клиента
        manager.updateClient(1, "Ivan", "Sidorov", "ivan.sidorov@example.com");

        // 9. Удаляем телефон
        manager.deletePhone(1, "+79991234567");

        // 10. Удаляем клиента
        manager.deleteClient(2);

        // 11. Ищем снова
        std::cout << "\nSearch for 'ivan':\n";
        clients = manager.findClients("ivan");
        for (const auto& client : clients) {
            std::cout << "ID: " << client.id << ", Name: " << client.first_name
                << ", Surname: " << client.last_name << ", Email: " << client.email
                << ", Phones: ";
            for (const auto& phone : client.phones) {
                std::cout << phone << " ";
            }
            std::cout << std::endl;
        }

    }
    catch (const pqxx::sql_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}