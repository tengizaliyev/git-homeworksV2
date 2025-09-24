#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <windows.h>
using namespace std;

int clients = 0;             
bool finished = false;
mutex mtx;            

void client(int max_clients) {
    for (int i = 0; i < max_clients; i++) {
        this_thread::sleep_for(1s);  
        lock_guard<mutex> lock(mtx);
        clients++;
        cout << "[Клиент] пришёл. Всего в очереди: " << clients << endl;
    }
    finished = true;
}

void operator_thread() {
    while (!finished || clients > 0) {
        this_thread::sleep_for(2s);  
        lock_guard<mutex> lock(mtx);
        if (clients > 0) {
            clients--;
            cout << "[Операционист] обслужил клиента. Осталось: " << clients << endl;
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
    int max_clients = 10; 

    thread t1(client, max_clients);
    thread t2(operator_thread);

    t1.join();
    t2.join();

    return 0;
}
