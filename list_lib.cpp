#include "list_lib.h"

#include <map>

List::List() : head(NULL), tail(NULL), count(0) {}

List::List(const std::vector<std::pair<std::string, int>> &arg) {
    head = NULL;
    tail = NULL;
    count = arg.size();
    ListNode* last = NULL;
    std::vector<ListNode*> ind;
    for (auto p : arg) {
        ListNode* cur = new ListNode();
        cur->data = p.first;
        ind.push_back(cur);
        if (last == NULL) {
            head = cur;
            last = cur;
        }
        else {
            last->next = cur;
            last = cur;
        }
    }
    tail = last;
    for (size_t i = 0; i < arg.size(); i++) {
        ind[i]->rand = ind[arg[i].second];
    }
}

void List::Clear() {
    ListNode* next = NULL;
    for (ListNode* current = head; current != NULL; current = next) {
        next = current->next;
        delete current;
    }
    head = NULL;
    tail = NULL;
    count = 0;
}

List::~List() {
    Clear();
}

bool List::IsEqual(const List &arg) {
    if (count != arg.count) {
        return false;
    }
    //List indexing
    std::map<ListNode*, int> ind;
    int i = 0;
    for (ListNode* current = head; current != NULL; current = current->next) {
        ind[current] = i++;
    }
    std::map<ListNode*, int> ind_arg;
    i = 0;
    for (ListNode* current = arg.head; current != NULL; current = current->next) {
        ind_arg[current] = i++;
    }
    //Comparison
    ListNode* x = head;
    ListNode* y = arg.head;
    while (x != NULL && y != NULL) {
        if (x->data != y->data || ind[x] != ind_arg[y]) {
            return false;
        }
        x = x->next;
        y = y->next;
    }
    return x == NULL && y == NULL;
}

std::string IntToBinaryString(int arg) {
    std::string result = "";
    for (int i = 0; i < 4; i++) {
        result += char(0);
    }
    if (arg < 0) {
        result[0] = char(128);
        arg += LONG_MIN;
    }
    int i = 3, j = 1;
    while (arg > 0) {
        result[i] |= j * (arg % 2);
        arg /= 2;
        if (j == 128) {
            j = 1;
            i -= 1;
        }
        else {
            j *= 2;
        }
    }
    return result;
}

void WriteInt(FILE* file, int value) {
    std::string it = IntToBinaryString(value);
    fwrite(it.c_str(), sizeof(char), 4, file);
}

void WriteChar(FILE* file, char value) {
    fwrite(&value, sizeof(char), 1, file);
}

void List::Serialize(FILE* file) {
    //List indexing
    std::map<ListNode*, int> ind;
    int i = 0;
    for (ListNode* current = head; current != NULL; current = current->next) {
        ind[current] = i++;
    }
    //Writting to file
    WriteInt(file, count);
    for (ListNode* current = head; current != NULL; current = current->next) {
        if (current->rand == NULL) {
            WriteInt(file, -1);
        }
        else {
            WriteInt(file, ind[current->rand]);
        }
        WriteInt(file, current->data.size());
        for (char c : current->data) {
            WriteChar(file, c);
        }
    }
}

int ReadInt(FILE* file) {
    char* buf = new char[4];
    fread(buf, sizeof(char), 4, file);
    int result = 1;
    if ((buf[0] >> 7) != 0) {
        result = -1;
        for (int i = 0; i < 4; i++) {
            buf[i] = ~buf[i];
        }
    }
    result *= buf[3] + buf[2] * 256 + buf[1] * 65536 + buf[0] * 16777216;
    delete[4] buf;
    return result;
}

char ReadChar(FILE* file) {
    char buf;
    fread(&buf, sizeof(char), 1, file);
    return buf;
}

void List::Deserialize(FILE* file) {
    //Clearing of content
    Clear();
    //Reading from file
    std::vector <int> rand;
    std::vector <ListNode*> adr;
    count = ReadInt(file);
    ListNode* last = NULL;
    for (int i = 0; i < count; i++) {
        ListNode* cur = new ListNode();
        adr.push_back(cur);
        rand.push_back(ReadInt(file));
        int strSize = ReadInt(file);
        std::string str = "";
        for (int j = 0; j < strSize; j++) {
            str += ReadChar(file);
        }
        cur->data = str;
        if (last == NULL) {
            head = cur;
        }
        else {
            last->next = cur;
            cur->prev = last;
        }
        last = cur;
    }
    if (last != NULL) {
        tail = last;
    }
    for (int i = 0; i < count; i++) {
        if (rand[i] != -1) {
            adr[i]->rand = adr[rand[i]];
        }
    }
}