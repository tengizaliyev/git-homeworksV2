// atomic_queue.cpp
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <windows.h>
using namespace std;

enum class MemOrder { seq_cst, acq_rel, relaxed };

inline memory_order mo_read(MemOrder o) {
    switch (o) {
    case MemOrder::acq_rel: return memory_order_acquire;
    case MemOrder::relaxed: return memory_order_relaxed;
    default:                return memory_order_seq_cst;
    }
}
inline memory_order mo_write(MemOrder o) {
    switch (o) {
    case MemOrder::acq_rel: return memory_order_release;
    case MemOrder::relaxed: return memory_order_relaxed;
    default:                return memory_order_seq_cst;
    }
}

atomic<int>  clients{ 0 };
atomic<bool> finished{ false };

void client(int max_clients, MemOrder order) {
    for (int i = 0; i < max_clients; ++i) {
        this_thread::sleep_for(1s);
        int after = clients.fetch_add(1, mo_write(order)) + 1;
        cout << "[Клиент] пришёл. Всего: " << after << '\n';
    }
    finished.store(true, mo_write(order));
}

void oper(MemOrder order) {
    while (!finished.load(mo_read(order)) || clients.load(mo_read(order)) > 0) {
        this_thread::sleep_for(2s);

        int cur = clients.load(mo_read(order));
        while (cur > 0) {
            if (clients.compare_exchange_weak(cur, cur - 1, mo_write(order), mo_read(order))) {
                cout << "[Операционист] обслужил. Осталось: " << (cur - 1) << '\n';
                break;
            }
        }
        if (cur == 0) {
            cout << "[Операционист] ждёт клиента...\n";
        }
    }
    cout << "[Операционист] закончил работу.\n";
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const int MAX_CLIENTS = 10;
    const MemOrder ORDER = MemOrder::seq_cst;

    thread t1(client, MAX_CLIENTS, ORDER);
    thread t2(oper, ORDER);

    t1.join();
    t2.join();
    return 0;
}
