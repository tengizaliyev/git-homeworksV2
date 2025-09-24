#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <windows.h>
using namespace std;

atomic<int> clients{ 0 };
atomic<bool> finished{ false };

void client(int max_clients) {
    for (int i = 0; i < max_clients; i++) {
        this_thread::sleep_for(1s);
        clients++;
        cout << "[Клиент] пришёл. Всего: " << clients.load() << endl;
    }
    finished = true;
}

void oper() {
    while (!finished || clients > 0) {
        this_thread::sleep_for(2s);
        if (clients > 0) {
            clients--;
            cout << "[Операционист] обслужил. Осталось: " << clients.load() << endl;
        }
        else {
            cout << "[Операционист] ждёт клиента..." << endl;
        }
    }
    cout << "[Операционист] закончил работу." << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int max_clients = 5;
    thread t1(client, max_clients);
    thread t2(oper);
    t1.join();
    t2.join();
    return 0;
}
