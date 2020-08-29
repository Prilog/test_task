#pragma once

#include <string>
#include <vector>

std::string IntToBinaryString(int arg);

struct ListNode {
    ListNode* prev;
    ListNode* next;
    ListNode* rand; // указатель на произвольный элемент данного списка либо NULL
    std::string data;
};

class List {
public:
    void Serialize(FILE* file); // сохранение в файл (файл открыт с помощью fopen(path, &quot; wb& quot;))
    void Deserialize(FILE* file); // загрузка из файла (файл открыт с помощью fopen(path, &quot; rb& quot;))

    //Custom section for testing
    List();
    List(const std::vector<std::pair<std::string, int>>& arg);
    ~List();

    bool IsEqual(const List& arg);

private:
    ListNode* head;
    ListNode* tail;
    int count;

    //Custom section for testing
    void Clear();
};