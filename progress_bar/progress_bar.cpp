// progress_bar.cpp
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

struct Console {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    void setPos(short x, short y) { COORD c{ x, y }; SetConsoleCursorPosition(h, c); }
    void color(WORD attr) { SetConsoleTextAttribute(h, attr); }
#else
    void setPos(short, short) {}
    void color(int) {}
#endif
} con;

mutex io_mx;

void printAt(short col, short row, const string& s, int color = 7) {
    lock_guard<mutex> lk(io_mx);
#ifdef _WIN32
    con.setPos(col, row);
    con.color((WORD)color);
#endif
    cout << s << flush;
#ifdef _WIN32
    con.color(7);
#endif
}

void worker(int idx, int barLen, int stepMs, short row) {
    {
        lock_guard<mutex> lk(io_mx);
#ifdef _WIN32
        con.setPos(0, row);
#endif
        cout << idx + 1 << "  " << this_thread::get_id()
            << "  |";
        for (int i = 0; i < barLen; ++i) cout << ' ';
        cout << "|  " << flush;
    }

    const short barStartCol = 20;
    auto start = chrono::steady_clock::now();

    mt19937 rng((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count() + idx);
    uniform_int_distribution<int> err(1, 12);

    for (int i = 0; i < barLen; ++i) {
        this_thread::sleep_for(chrono::milliseconds(stepMs));
        bool isError = (err(rng) == 1);
#ifdef _WIN32
        int color = isError ? 12 : 15;
#else
        int color = 7;
#endif
        printAt(barStartCol + i, row, string(1, isError ? '!' : '#'), color);
    }

    auto end = chrono::steady_clock::now();
    double secs = chrono::duration<double>(end - start).count();
    printAt(barStartCol + barLen + 3, row, to_string(secs) + "s");
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    const int NUM_THREADS = 5;
    const int BAR_LEN = 30; 
    const int STEP_MS = 300; 

    cout << "Idx  ThreadId           Progress Bar                  Time\n";
    cout << string(NUM_THREADS, '\n');

    vector<thread> ts;
    for (int i = 0; i < NUM_THREADS; ++i) {
        ts.emplace_back(worker, i, BAR_LEN, STEP_MS, (short)(i + 1));
    }
    for (auto& t : ts) t.join();

#ifdef _WIN32
    con.setPos(0, (short)(NUM_THREADS + 2));
    con.color(7);
#endif
    cout << "Done.\n";
    return 0;
}
