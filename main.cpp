#include "CQueue.h"
#include <iostream>
#include <cstring>
#include <queue>

#define N 100

using namespace std;

class myStr {
private:
    char *mem;

public:
    myStr() {
        mem = new char[N];
    }
    myStr(const char *k) {
        mem = new char[N];
        strcpy(mem, k);
    }
    myStr(const myStr& other) {
        mem = new char[N];
        strcpy(mem, other.mem);
    }
    myStr(myStr&& other) {
        mem = new char[N];
        strcpy(mem, other.mem);
        other.mem = nullptr;
    }
    ~myStr() {
        delete[] mem;
    }

    char& operator[](size_t i) {
        return mem[i];
    }
    friend ostream& operator<<(ostream& os, const myStr& my) {
        os << my.mem;
    }
};

int main() {
    CQueue<myStr> cq;
    myStr s1 = "233333";
    myStr s2 = "666666";
    myStr s3 = "114514";
    cq.push(s1);
    cq.push(s2);
    cq.push(s3);

    return 0;
}