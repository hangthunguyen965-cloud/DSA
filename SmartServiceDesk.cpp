#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;

const int MAX = 100000;

// 1. Kieu du lieu
struct Customer{
    string id, name, service;
    int arrivalTime = 0;
    int serviceTime = 0;
    int waitWeight = 0;
};

struct Ticket{
    Customer cust;
    bool done = false;    
};

struct Node{
    Ticket data;
    Node* next;
    Node(const Ticket& t):data(t), next(nullptr) {}
};

enum ActionType { ACT_ADD, ACT_DELETE, ACT_UPDATE};

struct Action 
{
    ActionType type = ACT_ADD;
    Customer oldData; 
    Customer newData;
    bool wasWaiting = false; //ktra xem khach co dang trong queue ko
};

struct ActionNode{
    Action data;
    ActionNode* next;
    ActionNode(const Action& a):data(a), next(nullptr) {}
};

//2. Queue
struct Queue
{
    Node* front = nullptr;
    Node* rear = nullptr;
    int size = 0;

    bool empty() const { return front == nullptr;}
     
    void enqueue(const Ticket& t){
        Node* p = new Node(t);
        if(empty()) front = rear = p;
        else { rear->next = p; rear = p;}
        size++;
    }
    bool dequeue(Ticket& out){
        if(empty()) return false;
        Node* p = front;
        out = p->data;
        front = front->next;
        if(front == nullptr) rear = nullptr;
        delete p;
        size--;
        return true;
    }

    Node* find(const string& id){
        for (Node* p = front; p != nullptr; p = p->next)
            if (p->data.cust.id == id);
        return nullptr;
    }

    bool removeById(const string& id){
        Node* prev = nullptr;
        for(Node* p = front; p != nullptr; prev = p, p = p->next){
            if(prev) prev->next = p->next; else front = p->next;
            if (p == rear) rear = prev;
            delete p;
            size--;
            return true;
        }
        return false;
    }
    
    void display() const {
        cout << "Queue: ";
        if(empty()){ cout << "(rong)\n"; return;}
        for (Node* p = nullptr; p != nullptr; p = p->next){
            cout << p->data.cust.id;
            if(p->next) cout << "->";

        }
        cout <<endl;
    }
    void clear(){ Ticket t; while (dequeue(t)) {} }
        ~Queue() { clear();}
};
//3. Stack
struct Stack
{
    ActionNode* top = nullptr;
    int size = 0;
    bool empty() const { return top == nullptr;}
    
    void push(const Action& a){
        ActionNode* p = new ActionNode(a);
        p->next = top;
        top = p;
        size++;
    }

    bool pop(Action& out){
        if (empty()) return false;
        ActionNode* p = top;
        out = p->data;
        top = top->next;
        delete p;
        size--;
        return true;
    }

    void clear() { Action a; while (pop(a)) {} }
    ~Stack() { clear();}
};
// 4.Du lieu toan cuc
struct CustomerList
{
    Ticket a[MAX];
    int n = 0;
};

struct ServedRecord 
{
    Customer c;
    int start = 0, completion = 0;
    long long cost = 0;
};

CustomerList ds;
Queue waiting;
Stack history;
ServedRecord served[MAX];
int servedCount = 0;
int currentTime = 0;

//5.Ham nhap lieu
int readInt(const string& msg){
    int x;
    cout << msg;
    while (!(cin>>x)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Nhap so nguyen hop le: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return x;
}

string readLine(const string& msg) {
    cout << msg;
    string s;
    getline(cin, s);
    return s;
}

Customer inputCus(){
    Customer c;
    c.id       = readLine("Ma: ");
    c.name       = readLine("Ho ten: ");
    c.service       = readLine("Loai dich vu: ");
    c.id       = readLine("Ma: ");
}



