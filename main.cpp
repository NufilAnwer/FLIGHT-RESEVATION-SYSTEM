#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <sstream>
#include <limits>

using namespace std;


class Customer;
class Flight;
class Queue;
class BST;


struct Time {
    int min;
    int hour;
};

// Discount types
enum class offers {
    NONE,
    STUDENT,
    SENIOR,
    MILITARY,
    FREQUENT_FLYER
};

template <typename T>
struct ListNode {
    T data;
    ListNode* prev;
    ListNode* next;

    ListNode(const T& val, ListNode* p = nullptr, ListNode* n = nullptr)
        : data(val), prev(p), next(n) {
    }
};

template <typename T>
class DoublyLinkedList {
private:
    ListNode<T>* head;
    ListNode<T>* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    
    ~DoublyLinkedList() { clear(); }

    DoublyLinkedList(const DoublyLinkedList& other) : head(nullptr), tail(nullptr), size(0) {
        ListNode<T>* current = other.head;
        while (current) {
            push_back(current->data);
            current = current->next;
        }
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            clear();
            ListNode<T>* current = other.head;
            while (current) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    void push_back(const T& data) {
        try {
            ListNode<T>* newNode = new ListNode<T>(data, tail, nullptr);
            if (tail) {
                tail->next = newNode;
            }
            else {
                head = newNode;
            }
            tail = newNode;
            size++;
        }
        catch (const std::bad_alloc& e) {
            std::cerr << "Memory allocation failed: " << e.what() << std::endl;
            throw;
        }
    }

    void erase(const ListNode<T>* node) {
        if (!node) return;

        if (node->prev) {
            node->prev->next = node->next;
        }
        else {
            head = node->next;
        }

        if (node->next) {
            node->next->prev = node->prev;
        }
        else {
            tail = node->prev;
        }

        delete node;
        size--;
    }

    void clear() {
        while (head) {
            ListNode<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    ListNode<T>* begin() const { return head; }
    ListNode<T>* end() const { return nullptr; }
    const ListNode<T>* cbegin() const { return head; }
    const ListNode<T>* cend() const { return nullptr; }

    bool empty() const { return size == 0; }
    int getSize() const { return size; }

    
    ListNode<T>* getTail() const { return tail; }

    ListNode<T>* getTailNode() const {
        return tail;
    }

    T& getTailData() {
        if (!tail) throw std::out_of_range("List is empty - cannot access tail data");
        return tail->data;
    }

    const T& getTailData() const {
        if (!tail) throw std::out_of_range("List is empty - cannot access tail data");
        return tail->data;
    }

    T& back() { return getTailData(); }
    const T& back() const { return getTailData(); }
};

class FlightBST {
private:
    struct Node {
        int key;
        Flight* flight;
        Node* left;
        Node* right;

        Node(int k, Flight* f) : key(k), flight(f), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void insert(Node*& node, int key, Flight* flight) {
        if (!node) {
            node = new Node(key, flight);
            return;
        }

        if (key < node->key) insert(node->left, key, flight);
        else insert(node->right, key, flight);
    }

    Node* find(Node* node, int key) const {
        if (!node) return nullptr;
        if (key == node->key) return node;
        if (key < node->key) return find(node->left, key);
        return find(node->right, key);
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

public:
    FlightBST() : root(nullptr) {}
    ~FlightBST() { clear(root); }

    void insert(int key, Flight* flight) { insert(root, key, flight); }

    Flight* find(int key) const {
        Node* node = find(root, key);
        return node ? node->flight : nullptr;
    }

    bool exists(int key) const { return find(root, key) != nullptr; }
};

class CustomerBST {
private:
    struct Node {
        int key; 
        Customer* customer;
        Node* left;
        Node* right;

        Node(int k, Customer* c) : key(k), customer(c), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void insert(Node*& node, int key, Customer* customer) {
        if (!node) {
            node = new Node(key, customer);
            return;
        }

        if (key < node->key) insert(node->left, key, customer);
        else insert(node->right, key, customer);
    }

    Node* find(Node* node, int key) const {
        if (!node) return nullptr;
        if (key == node->key) return node;
        if (key < node->key) return find(node->left, key);
        return find(node->right, key);
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    void inOrderTraversal(Node* node, DoublyLinkedList<Customer*>& result) const {
        if (node) {
            inOrderTraversal(node->left, result);
            result.push_back(node->customer);
            inOrderTraversal(node->right, result);
        }
    }

public:
    CustomerBST() : root(nullptr) {}
    ~CustomerBST() { clear(root); }

    void insert(int key, Customer* customer) { insert(root, key, customer); }

    Customer* find(int key) const {
        Node* node = find(root, key);
        return node ? node->customer : nullptr;
    }

    bool exists(int key) const { return find(root, key) != nullptr; }

    void erase(int key) {
        Node* parent = nullptr;
        Node* current = root;

        while (current && current->key != key) {
            parent = current;
            if (key < current->key) current = current->left;
            else current = current->right;
        }

        if (!current) return; 

        if (current->left && current->right) {
            Node* successor = current->right;
            Node* successorParent = current;

            while (successor->left) {
                successorParent = successor;
                successor = successor->left;
            }

            current->key = successor->key;
            current->customer = successor->customer;

            if (successorParent->left == successor) {
                successorParent->left = successor->right;
            }
            else {
                successorParent->right = successor->right;
            }
            delete successor;
        }
        // Case 2: Node has one child or no children
        else {
            Node* child = (current->left) ? current->left : current->right;

            if (!parent) { 
                root = child;
            }
            else {
                if (parent->left == current) {
                    parent->left = child;
                }
                else {
                    parent->right = child;
                }
            }
            delete current;
        }
    }

    DoublyLinkedList<Customer*> getAllCustomers() const {
        DoublyLinkedList<Customer*> result;
        inOrderTraversal(root, result);
        return result;
    }
};

class Flight {
private:
    int flightNo, cost, seats, booked_seats, availableSeats;
    string from, to, plane_type, weather, dayOfWeek;
    Time t_leave, t_arrive;

public:
    Flight() : flightNo(0), cost(0), seats(0), booked_seats(0) {}
    Flight(int no, int c, int s, int bs, string f, string t, string pt, Time tl, Time ta, string w, string d)
        : flightNo(no), cost(c), seats(s), booked_seats(bs), from(f), to(t), plane_type(pt), t_leave(tl), t_arrive(ta), weather(w), dayOfWeek(d) {
    }
    int getBookedSeats() const { return booked_seats; }
    static void updateFlight(int flightNumber);
     static void loadFromFile();
    static void saveToFile();

    // Flight management
    void addFlight();
    static void deleteFlight(int num);

    // Display functions
    void displayInfo() const;
    static void displayFlightInfo(int num);
    static void displaySchedule();

    // Booking functions
    static void resSeat(int num, int val);
    static bool checkForSeats(int num);
    static bool flightExists(int num);
    string getWeather() const { return weather; }
    string getDayOfWeek() const { return dayOfWeek; }
    void setWeather(const string& w) { weather = w; }
    void setDayOfWeek(const string& d) { dayOfWeek = d; }

    // Getters
    int getFlightNo() const { return flightNo; }
    int getCost() const { return cost; }
    Time getLeavingTime() const { return t_leave; }
    Time getArrivingTime() const { return t_arrive; }
    string getDeparture() const { return from; }
    string getDestination() const { return to; }
    string getPlaneType() const { return plane_type; }
    int getAvailableSeats() const { return seats - booked_seats; }
    void setAvailableSeats(int seats) {
        availableSeats = seats;
    }
    // Setters
    void setBookedSeats(int bs) { booked_seats = bs; }
};

// Customer class
class Customer {
private:
    int passportNo, tel;
    DoublyLinkedList<int> flights;
    string name, surname, nationality, address;
    offers discount;
    int totalSpent;

public:
    // Constructors
    Customer() : passportNo(0), tel(0), discount(offers::NONE), totalSpent(0) {}
    Customer(int pn, int t, string n, string s, string nat, string addr, offers d)
        : passportNo(pn), tel(t), name(n), surname(s), nationality(nat), address(addr), discount(d), totalSpent(0) {
    }
    void manageQueue(int num);
    // File operations
    static void loadFromFile();
    static void saveToFile();

    // Booking functions
    void book();
    void bookFromQueue(int num);
    void cancel();

    // Information functions
    static bool displayCustomerInfo(int passport);
    static bool uniquePass(int passport);

    // Helper functions
    void inputCustomeralInfo();
    void copyFromExisting(int passport);
    void applyDiscount();
    float getDiscountRate() const;

    // Getters
    int getPassport() const { return passportNo; }
    string getName() const { return name; }
    string getSurname() const { return surname; }
    string getNationality() const { return nationality; }
    string getAddress() const { return address; }
    int getTel() const { return tel; }
    offers getDiscount() const { return discount; }
    int getTotalSpent() const { return totalSpent; }
    const DoublyLinkedList<int>& getFlights() const { return flights; }

    // Setters
    void setTotalSpent(int amount) { totalSpent = amount; }
    void setPassport(int passport) { passportNo = passport; }
};


struct QueueNode {
    Customer Customer;
    QueueNode* next;
    time_t joinTime; 
};

// Queue class
class Queue {
private:
    int queueNo;
    QueueNode* front;
    QueueNode* rear;
    int size;

public:
    Queue(int queueId) : queueNo(queueId), front(nullptr), rear(nullptr), size(0) {}
    ~Queue() { clear(); }

    // File operations
    static void loadFromFile();
    static void saveToFile();
    void setSize(int newSize) { size = newSize; }
    // Queue operations
    void enqueue(const Customer& p);
    void dequeue();
    void clear();

    // Display functions
    static void display(int num);

    // Utility functions
    Customer oldest() const;
    bool isEmpty() const { return front == nullptr; }
    int getPosition(int passport) const;

    // Getters
    int getNo() const { return queueNo; }
    int getSize() const { return size; }
    QueueNode* getFront() const { return front; }
    QueueNode* getRear() const { return rear; }

    // Setters
    void setFront(QueueNode* f) { front = f; }
    void setRear(QueueNode* r) { rear = r; }
};

// Menu class
class Menu {
public:
    static void displayMenu();
    static void select(int selection);
    static void exit_prog();
};

// Input validation functions
bool checkTime(string time) {
    if (time.empty()) return false;
    if (time.length() != 5 || time[2] != ':') return false;

    for (int i = 0; i < time.length(); i++) {
        if (i == 2) {
            if (time[i] != ':') return false;
        }
        else if (!isdigit(time[i])) {
            return false;
        }
    }

    int hour = stoi(time.substr(0, 2));
    int min = stoi(time.substr(3, 2));
    return hour >= 0 && hour < 24 && min >= 0 && min < 60;
}

bool checkNumber(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool checkString(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

bool checkTime2(Time tLeaving, Time tArriving) {
    if (tLeaving.hour > tArriving.hour) {
        return true;
    }
    else if (tLeaving.hour == tArriving.hour) {
        if (tLeaving.min > tArriving.min) {
            return true;
        }
    }
    return false;
}

// Global data structures
DoublyLinkedList<Flight> flist;
CustomerBST customerBST;
DoublyLinkedList<Queue> qlist;
FlightBST flightBST;

// File operations
void Flight::loadFromFile() {
    ifstream file("flights.txt");
    if (!file.is_open()) return;

    flist.clear();
    flightBST = FlightBST(); // Reset BST

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        string token;
        Flight f;

        // Flight Number
        getline(iss, token, ',');
        if (!checkNumber(token)) continue;
        f.flightNo = stoi(token);

        // Departure
        getline(iss, f.from, ',');

        // Destination
        getline(iss, f.to, ',');

        // Boarding time
        getline(iss, token, ',');
        if (!checkTime(token)) continue;
        size_t colon = token.find(':');
        f.t_leave.hour = stoi(token.substr(0, colon));
        f.t_leave.min = stoi(token.substr(colon + 1));

        // Arrival time
        getline(iss, token, ',');
        if (!checkTime(token)) continue;
        colon = token.find(':');
        f.t_arrive.hour = stoi(token.substr(0, colon));
        f.t_arrive.min = stoi(token.substr(colon + 1));

        // Ticket price
        getline(iss, token, ',');
        if (!checkNumber(token)) continue;
        f.cost = stoi(token);

        // Plane type
        getline(iss, f.plane_type, ',');

        // Number of seats
        getline(iss, token, ',');
        if (!checkNumber(token)) continue;
        f.seats = stoi(token);

        // Booked seats
        getline(iss, token, ',');
        if (!checkNumber(token)) continue;
        f.booked_seats = stoi(token);

        // Weather
        getline(iss, f.weather, ',');

        // Day of week
        getline(iss, f.dayOfWeek);

        flist.push_back(f);
        flightBST.insert(f.flightNo, &flist.getTail()->data);

        // Create queue for this flight if it doesn't exist
        bool queueExists = false;
        for (ListNode<Queue>* qnode = qlist.begin(); qnode != qlist.end(); qnode = qnode->next) {
            if (f.flightNo == qnode->data.getNo()) {
                queueExists = true;
                break;
            }
        }
        if (!queueExists) {
            Queue q(f.flightNo);
            qlist.push_back(q);
        }
    }
    file.close();
}

void Flight::saveToFile() {
    ofstream file("flights.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not save flight data to file." << endl;
        return;
    }

    for (ListNode<Flight>* node = flist.begin(); node != flist.end(); node = node->next) {
        file << node->data.getFlightNo() << ","
            << node->data.getDeparture() << ","
            << node->data.getDestination() << ","
            << setw(2) << setfill('0') << node->data.getLeavingTime().hour << ":"
            << setw(2) << setfill('0') << node->data.getLeavingTime().min << ","
            << setw(2) << setfill('0') << node->data.getArrivingTime().hour << ":"
            << setw(2) << setfill('0') << node->data.getArrivingTime().min << ","
            << node->data.getCost() << ","
            << node->data.getPlaneType() << ","
            << node->data.getAvailableSeats() + node->data.getBookedSeats() << ","
            << node->data.getBookedSeats() << ","
            << node->data.getWeather() << ","
            << node->data.getDayOfWeek() << "\n";
    }
    file.close();
}

void Customer::loadFromFile() {
    ifstream file("passengers.txt");
    if (!file.is_open()) return;

    customerBST = CustomerBST(); // Reset BST

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        string token;
        Customer* p = new Customer();
        int discountValue;

        // Passport Number
        getline(iss, token, ',');
        if (!checkNumber(token)) {
            delete p;
            continue;
        }
        p->passportNo = stoi(token);

        // Name
        getline(iss, p->name, ',');

        // Surname
        getline(iss, p->surname, ',');

        // Nationality
        getline(iss, p->nationality, ',');

        // Address
        getline(iss, p->address, ',');

        // Telephone
        getline(iss, token, ',');
        if (!checkNumber(token)) {
            delete p;
            continue;
        }
        p->tel = stoi(token);

        // Discount Type
        getline(iss, token, ',');
        if (!checkNumber(token)) {
            delete p;
            continue;
        }
        discountValue = stoi(token);
        p->discount = static_cast<offers>(discountValue);

        // Total Spent
        getline(iss, token, ',');
        if (!checkNumber(token)) {
            delete p;
            continue;
        }
        p->totalSpent = stoi(token);

        // Flights
        string flightsStr;
        getline(iss, flightsStr);
        if (!flightsStr.empty()) {
            istringstream flightsStream(flightsStr);
            string flightToken;
            while (getline(flightsStream, flightToken, ';')) {
                if (checkNumber(flightToken)) {
                    p->flights.push_back(stoi(flightToken));
                }
            }
        }

        customerBST.insert(p->passportNo, p);
    }
    file.close();
}

void Customer::saveToFile() {
    ofstream file("passengers.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open passengers.txt for writing." << endl;
        return;
    }

    DoublyLinkedList<Customer*> customers = customerBST.getAllCustomers();
    for (ListNode<Customer*>* node = customers.begin(); node != customers.end(); node = node->next) {
        try {
            Customer* p = node->data;
            // Save basic customer info
            file << p->getPassport() << ","
                << p->getName() << ","
                << p->getSurname() << ","
                << p->getNationality() << ","
                << p->getAddress() << ","
                << p->getTel() << ","
                << static_cast<int>(p->getDiscount()) << ","
                << p->getTotalSpent();

            // Save flight list if not empty
            if (!p->getFlights().empty()) {
                file << ",";  // Separator before flight list

                bool firstFlight = true;
                ListNode<int>* fnode = p->getFlights().begin();

                // Safer iteration using while loop
                while (fnode != nullptr) {
                    if (fnode->data < 0) {
                        throw runtime_error("Invalid flight number: " + to_string(fnode->data));
                    }

                    if (!firstFlight) {
                        file << ";";
                    }
                    file << fnode->data;
                    firstFlight = false;

                    // Safely advance to next node
                    ListNode<int>* nextNode = fnode->next;
                    if (nextNode == fnode) {  // Detect circular reference
                        throw runtime_error("Circular reference in flight list");
                    }
                    fnode = nextNode;
                }
            }
            file << "\n";
        }
        catch (const exception& e) {
            cerr << "Error saving customer " << node->data->getPassport()
                << ": " << e.what() << endl;
            // Continue with next customer
        }
    }

    if (!file.good()) {
        cerr << "Error: Failed to write all data to file." << endl;
    }
    file.close();
}

void Queue::loadFromFile() {
    ifstream file("queues.txt");
    if (!file.is_open()) return;

    qlist.clear();

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        string token;

        // Queue Number
        getline(iss, token, ':');
        if (!checkNumber(token)) continue;
        int queueNo = stoi(token);

        // Find or create queue
        Queue* currentQueue = nullptr;
        for (ListNode<Queue>* qnode = qlist.begin(); qnode != qlist.end(); qnode = qnode->next) {
            if (queueNo == qnode->data.getNo()) {
                currentQueue = &qnode->data;
                break;
            }
        }
        if (!currentQueue) {
            Queue newQueue(queueNo);
            qlist.push_back(newQueue);
            currentQueue = &qlist.getTail()->data;
        }

        // Passengers in queue
        string passengerStr;
        getline(iss, passengerStr);
        if (!passengerStr.empty()) {
            istringstream passengerStream(passengerStr);
            string passportToken;
            while (getline(passengerStream, passportToken, ';')) {
                if (checkNumber(passportToken)) {
                    int passport = stoi(passportToken);
                    // Find the Customer in BST
                    Customer* customer = customerBST.find(passport);
                    if (customer) {
                        currentQueue->enqueue(*customer);
                    }
                }
            }
        }
    }
    file.close();
}

void Queue::saveToFile() {
    ofstream file("queues.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not save queue data to file." << endl;
        return;
    }

    for (ListNode<Queue>* qnode = qlist.begin(); qnode != qlist.end(); qnode = qnode->next) {
        file << qnode->data.getNo() << ":";

        bool firstPassenger = true;
        QueueNode* current = qnode->data.getFront();
        while (current) {
            if (!firstPassenger) file << ";";
            file << current->Customer.getPassport();
            firstPassenger = false;
            current = current->next;
        }
        file << "\n";
    }
    file.close();
}

// Flight member functions implementation
void Flight::addFlight() {
    string temp;
    bool flag = false;

    cout << "Add new flights by giving the following attributes:\n";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Flight Number
    do {
        cout << "Flight Number: ";
        getline(cin, temp);

        if (!checkNumber(temp)) {
            cout << "Please insert a valid Flight Number!\n";
            continue;
        }

        int num = stoi(temp);
        if (Flight::flightExists(num)) {
            cout << "This Flight already exists!\n";
            continue;
        }

        this->flightNo = num;
        break;
    } while (true);

    // Departure
    do {
        cout << "Departure: ";
        getline(cin, this->from);

        if (this->from.length() > 20 || !checkString(this->from)) {
            cout << "Please insert a valid Departure city!\n";
            continue;
        }
        break;
    } while (true);

    // Destination
    do {
        cout << "Destination: ";
        getline(cin, this->to);

        if (this->to.length() > 20 || !checkString(this->to) || this->to == this->from) {
            cout << "Please insert a valid Destination city!\n";
            continue;
        }
        break;
    } while (true);

    // Boarding time
    do {
        cout << "Boarding time (e.g. 19:40): ";
        getline(cin, temp);

        if (!checkTime(temp)) {
            cout << "Please insert a valid boarding time (e.g. 19:40)!\n";
            continue;
        }

        size_t colon = temp.find(':');
        this->t_leave.hour = stoi(temp.substr(0, colon));
        this->t_leave.min = stoi(temp.substr(colon + 1));
        break;
    } while (true);

    // Arrival time
    do {
        cout << "Arriving time (e.g. 21:40): ";
        getline(cin, temp);

        if (!checkTime(temp)) {
            cout << "Please insert a valid arriving time (e.g. 21:40)!\n";
            continue;
        }

        size_t colon = temp.find(':');
        this->t_arrive.hour = stoi(temp.substr(0, colon));
        this->t_arrive.min = stoi(temp.substr(colon + 1));

        if (!checkTime2(this->t_arrive, this->t_leave)) {
            cout << "Arrival time must be after departure time!\n";
            continue;
        }
        break;
    } while (true);

    // Ticket price
    do {
        cout << "Ticket price: ";
        getline(cin, temp);

        if (!checkNumber(temp)) {
            cout << "Please insert a valid ticket price!\n";
            continue;
        }
        this->cost = stoi(temp);
        break;
    } while (true);

    // Aeroplane type
    do {
        cout << "Aeroplane type: ";
        getline(cin, this->plane_type);

        if (this->plane_type.empty()) {
            cout << "Please insert a valid Aeroplane type!\n";
            continue;
        }
        break;
    } while (true);

    // Number of seats
    do {
        cout << "Number of seats: ";
        getline(cin, temp);

        if (!checkNumber(temp)) {
            cout << "Please insert a valid number of seats!\n";
            continue;
        }
        this->seats = stoi(temp);
        break;
    } while (true);

    // Number of booked seats
    do {
        cout << "Number of booked seats: ";
        getline(cin, temp);

        if (!checkNumber(temp)) {
            cout << "Please insert a valid number of booked seats!\n";
            continue;
        }

        int booked = stoi(temp);
        if (booked > this->seats) {
            cout << "Booked seats must be less than plane's seats!\n";
            continue;
        }
        this->booked_seats = booked;
        break;
    } while (true);
    do {
        cout << "Weather condition (e.g., Sunny, Rainy, Cloudy, etc.): ";
        getline(cin, this->weather);
        if (this->weather.empty()) {
            cout << "Please enter a valid weather condition!\n";
            continue;
        }
        break;
    } while (true);

    // Day of week
    do {
        cout << "Day of week (e.g., Monday, Tuesday, etc.): ";
        getline(cin, this->dayOfWeek);
        if (this->dayOfWeek.empty()) {
            cout << "Please enter a valid day of week!\n";
            continue;
        }
        break;
    } while (true);

    flist.push_back(*this);
    flightBST.insert(this->flightNo, &flist.getTail()->data);

    Queue q(this->flightNo);
    qlist.push_back(q);

    // Save to file
    Flight::saveToFile();
    Queue::saveToFile();

    cout << "Flight No: " << this->flightNo << " was successfully added!\n";
}

void Flight::deleteFlight(int num) {
    // Check if flight exists
    Flight* flight = flightBST.find(num);
    if (!flight) {
        cout << "Flight with number " << num << " does not exist!\n";
        return;
    }

    // Find the queue for this flight
    ListNode<Queue>* queueNodeToDelete = nullptr;
    Queue* flightQueue = nullptr;

    for (ListNode<Queue>* qnode = qlist.begin(); qnode != qlist.end(); qnode = qnode->next) {
        if (num == qnode->data.getNo()) {
            flightQueue = &qnode->data;
            queueNodeToDelete = qnode;
            break;
        }
    }

    // Handle passengers in queue
    if (flightQueue && !flightQueue->isEmpty()) {
        cout << "\nThere are " << flightQueue->getSize() << " passengers in the waiting queue for this flight.\n";
        cout << "You need to manage the queue before deleting this flight.\n";

        cout << "Options:\n";
        cout << "1. Manage queue now\n";
        cout << "2. Cancel deletion\n";
        cout << "Enter your choice (1-2): ";

        string choice;
        getline(cin, choice);

        while (choice != "1" && choice != "2") {
            cout << "Invalid choice. Please enter 1 or 2: ";
            getline(cin, choice);
        }

        if (choice == "1") {
            Customer customer;
            customer.manageQueue(num);
            // Check if queue is now empty
            if (flightQueue && !flightQueue->isEmpty()) {
                cout << "There are still " << flightQueue->getSize()
                    << " passengers in the queue. Cannot delete flight.\n";
                return;
            }
        }
        else {
            cout << "Flight deletion cancelled.\n";
            return;
        }
    }

    // Remove from flight list
    ListNode<Flight>* flightNodeToDelete = nullptr;
    for (ListNode<Flight>* fnode = flist.begin(); fnode != flist.end(); fnode = fnode->next) {
        if (num == fnode->data.getFlightNo()) {
            flightNodeToDelete = fnode;
            break;
        }
    }

    if (flightNodeToDelete) {
        flist.erase(flightNodeToDelete);
        flightBST = FlightBST(); // Rebuild BST without this flight

        // Rebuild flight BST
        for (ListNode<Flight>* fnode = flist.begin(); fnode != flist.end(); fnode = fnode->next) {
            flightBST.insert(fnode->data.getFlightNo(), &fnode->data);
        }
    }

    // Remove from queue list if exists
    if (queueNodeToDelete) {
        qlist.erase(queueNodeToDelete);
    }

    // Save changes to files
    Flight::saveToFile();
    Queue::saveToFile();

    cout << "\nFlight " << num << " has been successfully deleted.\n";

    // Update all customers who had this flight booked
    DoublyLinkedList<Customer*> customers = customerBST.getAllCustomers();
    for (ListNode<Customer*>* cnode = customers.begin(); cnode != customers.end(); cnode = cnode->next) {
        Customer* customer = cnode->data;
        bool modified = false;

        // Remove this flight from customer's bookings
        ListNode<int>* fnode = customer->getFlights().begin();
        while (fnode != nullptr) {
            ListNode<int>* nextNode = fnode->next; // Store next before possible deletion
            if (fnode->data == num) {
                DoublyLinkedList<int> flights = customer->getFlights();
                flights.erase(fnode);
                modified = true;
            }
            fnode = nextNode;
        }

        if (modified) {
            // Calculate refund (full price without discount as penalty)
            int refundAmount = flight->getCost();
            customer->setTotalSpent(customer->getTotalSpent() - refundAmount);

            cout << "Customer " << customer->getName() << " " << customer->getSurname()
                << " (Passport: " << customer->getPassport() << ") was booked on this flight.\n";
            cout << "Refund issued: $" << refundAmount << endl;
        }
    }

    // Save customer changes
    Customer::saveToFile();
}
void Customer::manageQueue(int flightNum) {
    // Find the flight
    Flight* flight = flightBST.find(flightNum);
    if (!flight) {
        cout << "Flight " << flightNum << " not found!\n";
        return;
    }

    // Find the queue for this flight
    Queue* flightQueue = nullptr;
    ListNode<Queue>* queueNode = nullptr;

    for (ListNode<Queue>* qnode = qlist.begin(); qnode != qlist.end(); qnode = qnode->next) {
        if (flightNum == qnode->data.getNo()) {
            flightQueue = &qnode->data;
            queueNode = qnode;
            break;
        }
    }

    if (!flightQueue || flightQueue->isEmpty()) {
        cout << "No customers in queue for Flight " << flightNum << ".\n";
        return;
    }

    cout << "\nManaging Queue for Flight " << flightNum << ":\n";
    cout << "Current seats: " << flight->getAvailableSeats() + flight->getBookedSeats() << endl;
    cout << "Available seats: " << flight->getAvailableSeats() << endl;
    cout << "Passengers in queue: " << flightQueue->getSize() << "\n\n";

    while (!flightQueue->isEmpty()) {
        // Display management options
        cout << "\n------------------------------------------------\n";
        cout << "1. Process next customer in queue\n";
      //  cout << "2. Increase available seats by 1\n";
        cout << "3. View queue\n";
        cout << "4. Exit queue management\n";
        cout << "Current available seats: " << flight->getAvailableSeats() << endl;
        cout << "Enter your choice (1,3,4): ";

        string choice;
        getline(cin, choice);

        if (choice == "1") {
            if (flight->getAvailableSeats() <= 0) {
                cout << "No available seats! Please increase seats first.\n";
                continue;
            }

            // Process next customer
            QueueNode* queueHead = flightQueue->getFront();
            if (!queueHead) break;

            int passport = queueHead->Customer.getPassport();
            Customer* customer = customerBST.find(passport);

            if (!customer) {
                flightQueue->dequeue();
                cout << "Removed invalid customer (Passport: " << passport << ") from queue.\n";
                continue;
            }

            // Display customer info
            cout << "\nNext customer in queue:\n";
            cout << "Name: " << customer->getName() << " " << customer->getSurname() << endl;
            cout << "Passport: " << customer->getPassport() << endl;
            cout << "Time in queue: " << difftime(time(nullptr), queueHead->joinTime) / 60 << " minutes\n";

            // Calculate price
            float discountRate = customer->getDiscountRate();
            int originalPrice = flight->getCost();
            int discountedPrice = originalPrice * (1 - discountRate);

            cout << "Ticket Price: $" << discountedPrice;
            if (discountRate > 0) {
                cout << " (Discount: " << (discountRate * 100) << "%)";
            }
            cout << endl;

            cout << "Confirm allocation? (y/n): ";
            string confirm;
            getline(cin, confirm);

            if (confirm == "y" || confirm == "Y") {
                // Book the flight
                customer->flights.push_back(flightNum);
                flight->setBookedSeats(flight->getBookedSeats() + 1);
                customer->setTotalSpent(customer->getTotalSpent() + discountedPrice);

                // Remove from queue
                flightQueue->dequeue();

                cout << "\nSeat allocated successfully to " << customer->getName() << "!\n";
                cout << "Amount charged: $" << discountedPrice << endl;

                if (flightQueue->isEmpty()) {
                    cout << "Queue for Flight " << flightNum << " is now empty.\n";
                }
            }
            else {
                cout << "Allocation cancelled.\n";
            }
        }
        else if (choice == "2") {
            // Increase available seats
            flight->setAvailableSeats(flight->getAvailableSeats() + 1);
            cout << "Added 1 seat to Flight " << flightNum << ".\n";
            cout << "Total seats now: " << flight->getAvailableSeats() + flight->getBookedSeats() << endl;
            cout << "Available seats now: " << flight->getAvailableSeats() << endl;

            // Save the seat change immediately
            Flight::saveToFile();
        }
        else if (choice == "3") {
            // View current queue
            cout << "\nCurrent queue for Flight " << flightNum << ":\n";
            cout << left << setw(5) << "Pos" << setw(25) << "Name" << setw(15) << "Passport"
                << setw(15) << "Wait Time" << setw(20) << "Discount" << endl;

            int pos = 1;
            QueueNode* current = flightQueue->getFront();
            while (current) {
                string discountStr;
                switch (current->Customer.getDiscount()) {
                case offers::STUDENT: discountStr = "Student (15%)"; break;
                case offers::SENIOR: discountStr = "Senior (20%)"; break;
                case offers::MILITARY: discountStr = "Military (10%)"; break;
                case offers::FREQUENT_FLYER: discountStr = "Frequent Flyer (25%)"; break;
                case offers::NONE: discountStr = "None"; break;
                }

                cout << left << setw(5) << pos++
                    << setw(25) << (current->Customer.getName() + " " + current->Customer.getSurname())
                    << setw(15) << current->Customer.getPassport()
                    << setw(15) << difftime(time(nullptr), current->joinTime) / 60 << " mins"
                    << setw(20) << discountStr << endl;
                current = current->next;
            }
        }
        else if (choice == "4") {
            cout << "Exiting queue management.\n";
            break;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    // If queue is empty after processing, remove it
    if (flightQueue && flightQueue->isEmpty() && queueNode) {
        qlist.erase(queueNode);
        cout << "Queue for Flight " << flightNum << " has been removed as it's now empty.\n";
    }

    // Save all changes
    Flight::saveToFile();
    Customer::saveToFile();
    Queue::saveToFile();

    cout << "\nQueue management for Flight " << flightNum << " completed.\n";
}

void Flight::displayInfo() const {
    cout << left << setw(10) << flightNo
        << left << setw(15) << from
        << left << setw(15) << to
        << left << setw(10) << (to_string(t_leave.hour) + ":" + (t_leave.min < 10 ? "0" : "") + to_string(t_leave.min))
        << left << setw(10) << (to_string(t_arrive.hour) + ":" + (t_arrive.min < 10 ? "0" : "") + to_string(t_arrive.min))
        << left << setw(10) << "$" + to_string(cost)
        << left << setw(15) << plane_type
        << left << setw(10) << seats
        << left << setw(10) << booked_seats
        << left << setw(10) << weather
        << left << setw(10) << dayOfWeek << endl;
}

void Flight::displayFlightInfo(int num) {
    Flight* flight = flightBST.find(num);
    if (flight) {
        cout << left << setw(10) << "FLIGHT" << left << setw(15) << "FROM" << left << setw(15) << "TO"
            << left << setw(10) << "LEAVE" << left << setw(10) << "ARRIVE" << left << setw(10) << "COST"
            << left << setw(15) << "TYPE" << left << setw(10) << "SEATS" << left << setw(10) << "BOOKED" << endl;
        flight->displayInfo();
        Queue::display(num);
    }
    else {
        cout << "Invalid number of flight was given.\n";
    }
}

void Flight::displaySchedule() {
    cout << "\n\t\t\t\t FLIGHT SCHEDULE\n\n";
    cout << left << setw(10) << "FLIGHT" << left << setw(15) << "FROM" << left << setw(15) << "TO"
        << left << setw(10) << "LEAVE" << left << setw(10) << "ARRIVE" << left << setw(10) << "COST"
        << left << setw(15) << "TYPE" << left << setw(10) << "SEATS" << left << setw(10) << "BOOKED"
        << left << setw(10) << "WEATHER" << left << setw(10) << "DAY" << endl;

    for (ListNode<Flight>* node = flist.begin(); node != flist.end(); node = node->next) {
        node->data.displayInfo();
    }
    cout << endl;
}

void Flight::resSeat(int num, int val) {
    Flight* flight = flightBST.find(num);
    if (flight) {
        flight->booked_seats += val;
        Flight::saveToFile(); // Save changes to file
    }
}

bool Flight::flightExists(int num) {
    return flightBST.exists(num);
}

bool Flight::checkForSeats(int num) {
    Flight* flight = flightBST.find(num);
    return flight ? (flight->seats > flight->booked_seats) : false;
}

// Customer member functions implementation
void Customer::applyDiscount() {
    string choice;
    cout << "\nAvailable Discounts:\n";
    cout << "1. Student (15% discount)\n";
    cout << "2. Senior Citizen (20% discount)\n";
    cout << "3. Military (10% discount)\n";
    cout << "4. Frequent Flyer (25% discount)\n";
    cout << "5. No discount\n";
    cout << "Enter your choice (1-5): ";

    getline(cin, choice);
    while (choice.empty() || choice[0] < '1' || choice[0] > '5') {
        cout << "Invalid choice. Please enter a number between 1 and 5: ";
        getline(cin, choice);
    }

    switch (choice[0]) {
    case '1': discount = offers::STUDENT; break;
    case '2': discount = offers::SENIOR; break;
    case '3': discount = offers::MILITARY; break;
    case '4': discount = offers::FREQUENT_FLYER; break;
    case '5': discount = offers::NONE; break;
    }
}

float Customer::getDiscountRate() const {
    switch (discount) {
    case offers::STUDENT: return 0.15f;
    case offers::SENIOR: return 0.20f;
    case offers::MILITARY: return 0.10f;
    case offers::FREQUENT_FLYER: return 0.25f;
    case offers::NONE: return 0.0f;
    }
    return 0.0f;
}

void Customer::inputCustomeralInfo() {
    string temp;

    // Name
    do {
        cout << "Name: ";
        getline(cin, this->name);
        if (this->name.length() > 20 || !checkString(this->name)) {
            cout << "Please insert a valid Name (letters only, max 20 chars)!\n";
            continue;
        }
        break;
    } while (true);

    // Surname
    do {
        cout << "Surname: ";
        getline(cin, this->surname);
        if (this->surname.length() > 20 || !checkString(this->surname)) {
            cout << "Please insert a valid Surname (letters only, max 20 chars)!\n";
            continue;
        }
        break;
    } while (true);

    // Passport number
    do {
        cout << "Passport number: ";
        getline(cin, temp);
        if (!checkNumber(temp) || !Customer::uniquePass(stoi(temp))) {
            cout << "Please insert a valid and unique passport number!\n";
            continue;
        }
        this->passportNo = stoi(temp);
        break;
    } while (true);

    // Nationality
    do {
        cout << "Nationality: ";
        getline(cin, this->nationality);
        if (this->nationality.length() > 20 || !checkString(this->nationality)) {
            cout << "Please insert a valid Nationality (letters only, max 20 chars)!\n";
            continue;
        }
        break;
    } while (true);

    // Address
    do {
        cout << "Address: ";
        getline(cin, this->address);
        if (this->address.empty()) {
            cout << "Address cannot be empty!\n";
            continue;
        }
        break;
    } while (true);

    // Telephone
    do {
        cout << "Telephone: ";
        getline(cin, temp);
        if (!checkNumber(temp)) {
            cout << "Please insert a valid telephone number!\n";
            continue;
        }
        this->tel = stoi(temp);
        break;
    } while (true);

    // Apply discount
    applyDiscount();
}

void Customer::copyFromExisting(int passport) {
    Customer* existing = customerBST.find(passport);
    if (existing) {
        this->name = existing->name;
        this->surname = existing->surname;
        this->passportNo = existing->passportNo;
        this->nationality = existing->nationality;
        this->address = existing->address;
        this->tel = existing->tel;
        this->discount = existing->discount;
        this->totalSpent = existing->totalSpent;

        // Copy flights
        for (ListNode<int>* fnode = existing->flights.begin(); fnode != existing->flights.end(); fnode = fnode->next) {
            this->flights.push_back(fnode->data);
        }

        // Remove from BST
        customerBST.erase(passport);
        delete existing;
    }
}

void Customer::book() {
    if (flist.empty()) {
        cout << "There are no available flights at the moment.\n";
        return;
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string choice;
    cout << "Insert y (yes) for a new client or n (no) for an existing client: ";
    getline(cin, choice);

    if (choice == "y" || choice == "Y") {
        cout << "Please provide your personal information.\n";
        inputCustomeralInfo();
    }
    else {
        string temp;
        cout << "Please provide your passport number: ";
        getline(cin, temp);

        while (!checkNumber(temp)) {
            cout << "Please insert a valid passport number!\n";
            getline(cin, temp);
        }

        int passport = stoi(temp);
        if (!Customer::uniquePass(passport)) {
            copyFromExisting(passport);
        }
        else {
            cout << "Wrong passport number!\n";
            return;
        }
    }

    Flight::displaySchedule();

    string temp;
    int num;
    do {
        cout << "\n\nEnter the number of the flight you're interested in: ";
        getline(cin, temp);

        if (!checkNumber(temp) || !Flight::flightExists(stoi(temp))) {
            cout << "Please insert a valid Flight No!\n";
            continue;
        }
        num = stoi(temp);
        break;
    } while (true);

    Flight* flight = flightBST.find(num);
    if (!flight) {
        cout << "Flight not found!\n";
        return;
    }

    if (flight->getAvailableSeats() > 0) {
        float discountRate = getDiscountRate();
        int originalPrice = flight->getCost();
        int discountedPrice = originalPrice * (1 - discountRate);

        cout << "\nFlight Details:\n";
        cout << "From: " << flight->getDeparture() << " To: " << flight->getDestination() << "\n";
        cout << "Departure: " << flight->getLeavingTime().hour << ":"
            << (flight->getLeavingTime().min < 10 ? "0" : "") << flight->getLeavingTime().min << "\n";
        cout << "Original Price: $" << originalPrice << "\n";

        if (discountRate > 0) {
            cout << "Discount Applied: " << (discountRate * 100) << "%\n";
            cout << "Discounted Price: $" << discountedPrice << "\n";
        }

        cout << "\nConfirm booking? (y/n): ";
        getline(cin, choice);

        if (choice == "y" || choice == "Y") {
            flights.push_back(num);
            Flight::resSeat(num, 1);
            totalSpent += discountedPrice;

            // Add to BST
            Customer* newCustomer = new Customer(*this);
            customerBST.insert(newCustomer->getPassport(), newCustomer);

            // Save to file
            Customer::saveToFile();

            cout << "Your flight with No: " << num << " was successfully booked.\n";
            cout << "Total amount paid: $" << discountedPrice << "\n";
        }
        else {
            cout << "Booking cancelled.\n";
        }
    }
    else {
        cout << "\nThis flight is fully booked. You can be added to the waiting queue.\n";
        cout << "Would you like to join the waiting queue? (y/n): ";
        getline(cin, choice);

        if (choice == "y" || choice == "Y") {
            // Find or create queue for this flight
            Queue* flightQueue = nullptr;
            for (ListNode<Queue>* qnode = qlist.begin(); qnode != qlist.end(); qnode = qnode->next) {
                if (num == qnode->data.getNo()) {
                    flightQueue = &qnode->data;
                    break;
                }
            }

            if (!flightQueue) {
                // Create new queue if it doesn't exist
                Queue newQueue(num);
                qlist.push_back(newQueue);
                flightQueue = &qlist.getTail()->data;
            }

            // Add customer to queue
            flightQueue->enqueue(*this);
            int queuePosition = flightQueue->getSize();

            // Add to BST
            Customer* newCustomer = new Customer(*this);
            customerBST.insert(newCustomer->getPassport(), newCustomer);

            // Save to file
            Customer::saveToFile();
            Queue::saveToFile();

            cout << "\nYou have been added to the waiting queue for Flight " << num << ".\n";
            cout << "Your position in queue: " << queuePosition << endl;
            cout << "You will be automatically notified if a seat becomes available.\n";

            // Check if we can immediately allocate a seat (in case of race condition)
            if (flight->getAvailableSeats() > 0) {
                cout << "\nSeat became available! Processing your booking...\n";
                manageQueue(num);
            }
        }
        else {
            cout << "You were not added to the waiting queue.\n";
        }
    }
}

void Customer::bookFromQueue(int num) {
    Customer queueFront;
    bool found = false;

    // Find the oldest customer in the queue
    for (ListNode<Queue>* qnode = qlist.begin(); qnode != qlist.end(); qnode = qnode->next) {
        if (num == qnode->data.getNo() && !qnode->data.isEmpty()) {
            queueFront = qnode->data.oldest();
            found = true;
            break;
        }
    }

    if (!found) return;

    // Check if all flights have available seats
    bool allAvailable = true;
    for (ListNode<int>* node = queueFront.flights.begin(); node != queueFront.flights.end(); node = node->next) {
        if (!Flight::checkForSeats(node->data)) {
            allAvailable = false;
            break;
        }
    }

    if (allAvailable) {
        // Remove from queues and book flights
        for (ListNode<int>* node = queueFront.flights.begin(); node != queueFront.flights.end(); node = node->next) {
            for (ListNode<Queue>* qnode = qlist.begin(); qnode != qlist.end(); qnode = qnode->next) {
                if (node->data == qnode->data.getNo()) {
                    QueueNode* qfront = qnode->data.getFront();
                    while (qfront) {
                        if (queueFront.passportNo == qfront->Customer.passportNo) {
                            qnode->data.dequeue();

                            // Apply discount
                            Flight* flight = flightBST.find(node->data);
                            if (flight) {
                                float discountRate = queueFront.getDiscountRate();
                                int originalPrice = flight->getCost();
                                int discountedPrice = originalPrice * (1 - discountRate);

                                // Update Customer's total spent
                                Customer* customer = customerBST.find(queueFront.passportNo);
                                if (customer) {
                                    customer->setTotalSpent(customer->getTotalSpent() + discountedPrice);
                                }
                            }

                            Flight::resSeat(node->data, 1);
                            break;
                        }
                        qfront = qfront->next;
                    }
                    break;
                }
            }
        }

        // Save changes to files
        Customer::saveToFile();
        Queue::saveToFile();
    }
}

void Customer::cancel() {
    if (customerBST.getAllCustomers().empty()) {
        cout << "There are no registered clients at the moment!\n";
        return;
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string passportStr, flightStr;
    int passport, flightNum;
    int counter = 1;

    // Get passport number
    do {
        cout << "Please provide your passport number: ";
        getline(cin, passportStr);

        if (!checkNumber(passportStr)) {
            cout << "Please insert a valid passport number!\n";
            continue;
        }

        passport = stoi(passportStr);
        if (!Customer::displayCustomerInfo(passport)) {
            if (counter == 5) {
                cout << "Wrong passport number was given too many times.\n";
                return;
            }
            cout << "Please check your Passport Number.\n";
            counter++;
            continue;
        }
        break;
    } while (true);

    // Get flight number to cancel
    counter = 1;
    bool flightFound = false;
    do {
        cout << "\nWhich flight do you want to cancel (enter the flight number)? ";
        getline(cin, flightStr);

        if (!checkNumber(flightStr)) {
            cout << "Please insert a valid flight number!\n";
            continue;
        }
        flightNum = stoi(flightStr);

        // Find the Customer
        Customer* customer = customerBST.find(passport);
        if (customer) {
            // Find the flight in Customer's flights
            for (ListNode<int>* fnode = customer->flights.begin(); fnode != customer->flights.end(); fnode = fnode->next) {
                if (flightNum == fnode->data) {
                    // Calculate refund with discount
                    Flight* flight = flightBST.find(flightNum);
                    if (flight) {
                        float discountRate = customer->getDiscountRate();
                        int originalPrice = flight->getCost();
                        int refundAmount = originalPrice * (1 - discountRate);

                        customer->setTotalSpent(customer->getTotalSpent() - refundAmount);
                        cout << "Refund amount: $" << refundAmount << "\n";
                    }

                    customer->flights.erase(fnode);
                    flightFound = true;
                    break;
                }
            }
        }

        if (counter == 5) {
            cout << "Wrong flight number was given too many times.\n";
            return;
        }
        else if (!flightFound) {
            cout << "Please check your flight number!.\n";
            counter++;
            continue;
        }
        break;
    } while (true);

    // Check if Customer is in any queue
    bool isInQueue = false;
    for (ListNode<Queue>* qnode = qlist.begin(); qnode != qlist.end(); qnode = qnode->next) {
        if (flightNum == qnode->data.getNo() && !qnode->data.isEmpty()) {
            QueueNode* prev = nullptr;
            QueueNode* current = qnode->data.getFront();

            while (current) {
                if (passport == current->Customer.getPassport()) {
                    isInQueue = true;

                    if (current == qnode->data.getFront()) {
                        qnode->data.setFront(current->next);
                        if (!qnode->data.getFront()) {
                            qnode->data.setRear(nullptr);
                        }
                    }
                    else if (current == qnode->data.getRear()) {
                        prev->next = nullptr;
                        qnode->data.setRear(prev);
                    }
                    else {
                        prev->next = current->next;
                    }

                    delete current;
                    //qnode->data.q--;  // Maintain size
                    qnode->data.setSize(qnode->data.getSize() - 1);
                    break;
                }
                prev = current;
                current = current->next;
            }
        }
    }

    if (!isInQueue) {
        Flight::resSeat(flightNum, -1);
        bookFromQueue(flightNum);
    }

    // Save changes to files
    Customer::saveToFile();
    Queue::saveToFile();

    cout << "\nYour reservation was successfully removed!\n\n";
}

bool Customer::displayCustomerInfo(int passport) {
    Customer* customer = customerBST.find(passport);
    if (customer) {
        cout << "\n\t\t\t\t CustomerAL INFO\n\n";
        cout << left << setw(15) << "NAME" << left << setw(15) << "SURNAME" << left << setw(15) << "PASSPORT_No"
            << left << setw(15) << "NATIONALITY" << left << setw(15) << "ADDRESS" << left << setw(15) << "TELEPHONE"
            << left << setw(20) << "DISCOUNT" << left << setw(15) << "TOTAL SPENT" << endl;

        string discountStr;
        switch (customer->discount) {
        case offers::STUDENT: discountStr = "Student (15%)"; break;
        case offers::SENIOR: discountStr = "Senior (20%)"; break;
        case offers::MILITARY: discountStr = "Military (10%)"; break;
        case offers::FREQUENT_FLYER: discountStr = "Frequent Flyer (25%)"; break;
        case offers::NONE: discountStr = "None"; break;
        }

        cout << left << setw(15) << customer->name << left << setw(15) << customer->surname
            << left << setw(15) << customer->passportNo << left << setw(15) << customer->nationality
            << left << setw(15) << customer->address << left << setw(15) << customer->tel
            << left << setw(20) << discountStr << left << setw(15) << "$" + to_string(customer->totalSpent) << endl << endl;

        cout << "Your flight(s): ";
        for (ListNode<int>* fnode = customer->flights.begin(); fnode != customer->flights.end(); fnode = fnode->next) {
            cout << fnode->data << "  ";
        }
        cout << endl;
        return true;
    }
    cout << "Passport number was not found. Check again!\n";
    return false;
}

bool Customer::uniquePass(int passport) {
    return !customerBST.exists(passport);
}

void Queue::enqueue(const Customer& p) {
    QueueNode* newNode = new QueueNode{ p, nullptr, time(nullptr) };
    size++;  

    if (!rear) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
}

void Queue::dequeue() {
    if (!front) return;

    QueueNode* temp = front;
    front = front->next;
    size--;  // Maintain size

    if (!front) {
        rear = nullptr;
    }

    delete temp;
}

void Queue::clear() {
    while (front) {
        QueueNode* temp = front;
        front = front->next;
        delete temp;
    }
    rear = nullptr;
    size = 0;
}

Customer Queue::oldest() const {
    if (front) {
        return front->Customer;
    }
    throw runtime_error("Queue is empty");
}

int Queue::getPosition(int passport) const {
    int position = 1;
    QueueNode* current = front;
    while (current) {
        if (current->Customer.getPassport() == passport) {
            return position;
        }
        current = current->next;
        position++;
    }
    return -1; // Not found
}

void Queue::display(int num) {
    for (ListNode<Queue>* node = qlist.begin(); node != qlist.end(); node = node->next) {
        if (num == node->data.getNo()) {
            QueueNode* current = node->data.getFront();

            if (!current) {
                cout << "\nEmpty queue.\n";
            }
            else {
                cout << "\nPassengers waiting in queue:\n";
                cout << left << setw(15) << "Passport" << left << setw(15) << "Name" << left << setw(15) << "Surname"
                    << left << setw(20) << "Discount" << left << setw(15) << "Wait Time" << endl;

                while (current) {
                    string discountStr;
                    switch (current->Customer.getDiscount()) {
                    case offers::STUDENT: discountStr = "Student (15%)"; break;
                    case offers::SENIOR: discountStr = "Senior (20%)"; break;
                    case offers::MILITARY: discountStr = "Military (10%)"; break;
                    case offers::FREQUENT_FLYER: discountStr = "Frequent Flyer (25%)"; break;
                    case offers::NONE: discountStr = "None"; break;
                    }

                    cout << left << setw(15) << current->Customer.getPassport()
                        << left << setw(15) << current->Customer.getName()
                        << left << setw(15) << current->Customer.getSurname()
                        << left << setw(20) << discountStr
                        << left << setw(15) << difftime(time(nullptr), current->joinTime) / 60 << " mins" << endl;
                    current = current->next;
                }
            }
            return;
        }
    }
}
void Flight::updateFlight(int flightNo) {
    string temp;
    Flight* flight = flightBST.find(flightNo);

    if (!flight) {
        cout << "Flight with number " << flightNo << " does not exist!\n";
        return;
    }

    cout << "Update flight with the following attributes (leave blank to keep current value):\n";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Departure
    do {
        cout << "Current Departure: " << flight->from << "\n";
        cout << "Departure: ";
        getline(cin, temp);

        if (!temp.empty()) {
            if (temp.length() > 20 || !checkString(temp)) {
                cout << "Please insert a valid Departure city!\n";
                continue;
            }
            flight->from = temp;
        }
        break;
    } while (true);

    // Destination
    do {
        cout << "Current Destination: " << flight->to << "\n";
        cout << "Destination: ";
        getline(cin, temp);

        if (!temp.empty()) {
            if (temp.length() > 20 || !checkString(temp) || temp == flight->from) {
                cout << "Please insert a valid Destination city!\n";
                continue;
            }
            flight->to = temp;
        }
        break;
    } while (true);

    // Boarding time
    do {
        cout << "Current Boarding time: "
            << setw(2) << setfill('0') << flight->t_leave.hour << ":"
            << setw(2) << setfill('0') << flight->t_leave.min << "\n";
        cout << "Boarding time (e.g. 19:40): ";
        getline(cin, temp);

        if (!temp.empty()) {
            if (!checkTime(temp)) {
                cout << "Please insert a valid boarding time (e.g. 19:40)!\n";
                continue;
            }

            size_t colon = temp.find(':');
            Time newTime;
            newTime.hour = stoi(temp.substr(0, colon));
            newTime.min = stoi(temp.substr(colon + 1));

            if (checkTime2( flight->t_arrive,newTime)) {
                cout << "Boarding time must be before arrival time!\n";
                continue;
            }
            flight->t_leave = newTime;
        }
        break;
    } while (true);

    // Arrival time
    do {
        cout << "Current Arriving time: "
            << setw(2) << setfill('0') << flight->t_arrive.hour << ":"
            << setw(2) << setfill('0') << flight->t_arrive.min << "\n";
        cout << "Arriving time (e.g. 21:40): ";
        getline(cin, temp);

        if (!temp.empty()) {
            if (!checkTime(temp)) {
                cout << "Please insert a valid arriving time (e.g. 21:40)!\n";
                continue;
            }

            size_t colon = temp.find(':');
            Time newTime;
            newTime.hour = stoi(temp.substr(0, colon));
            newTime.min = stoi(temp.substr(colon + 1));

            if (checkTime2(flight->t_leave, newTime)) {
                cout << "Arrival time must be after departure time!\n";
                continue;
            }
            flight->t_arrive = newTime;
        }
        break;
    } while (true);

    // Ticket price
    do {
        cout << "Current Ticket price: " << flight->cost << "\n";
        cout << "Ticket price: ";
        getline(cin, temp);

        if (!temp.empty()) {
            if (!checkNumber(temp)) {
                cout << "Please insert a valid ticket price!\n";
                continue;
            }
            flight->cost = stoi(temp);
        }
        break;
    } while (true);

    // Aeroplane type
    do {
        cout << "Current Aeroplane type: " << flight->plane_type << "\n";
        cout << "Aeroplane type: ";
        getline(cin, temp);

        if (!temp.empty()) {
            if (temp.empty()) {
                cout << "Please insert a valid Aeroplane type!\n";
                continue;
            }
            flight->plane_type = temp;
        }
        break;
    } while (true);

    // Number of seats
    do {
        cout << "Current Number of seats: " << flight->seats << "\n";
        cout << "Number of seats: ";
        getline(cin, temp);

        if (!temp.empty()) {
            if (!checkNumber(temp)) {
                cout << "Please insert a valid number of seats!\n";
                continue;
            }
            int newSeats = stoi(temp);
            if (newSeats < flight->booked_seats) {
                cout << "Cannot have fewer seats than currently booked (" << flight->booked_seats << ")!\n";
                continue;
            }
            flight->seats = newSeats;
        }
        break;
    } while (true);

    // Number of booked seats
    do {
        cout << "Current Number of booked seats: " << flight->booked_seats << "\n";
        cout << "Number of booked seats: ";
        getline(cin, temp);

        if (!temp.empty()) {
            if (!checkNumber(temp)) {
                cout << "Please insert a valid number of booked seats!\n";
                continue;
            }

            int booked = stoi(temp);
            if (booked > flight->seats) {
                cout << "Booked seats must be less than plane's seats!\n";
                continue;
            }
            flight->booked_seats = booked;
        }
        break;
    } while (true);

    // Weather condition
    do {
        cout << "Current Weather condition: " << flight->weather << "\n";
        cout << "Weather condition (e.g., Sunny, Rainy, Cloudy, etc.): ";
        getline(cin, temp);

        if (!temp.empty()) {
            if (temp.empty()) {
                cout << "Please enter a valid weather condition!\n";
                continue;
            }
            flight->weather = temp;
        }
        break;
    } while (true);

    // Day of week
    do {
        cout << "Current Day of week: " << flight->dayOfWeek << "\n";
        cout << "Day of week (e.g., Monday, Tuesday, etc.): ";
        getline(cin, temp);

        if (!temp.empty()) {
            if (temp.empty()) {
                cout << "Please enter a valid day of week!\n";
                continue;
            }
            flight->dayOfWeek = temp;
        }
        break;
    } while (true);

    // Save to file
    Flight::saveToFile();
    cout << "Flight No: " << flightNo << " was successfully updated!\n";
}
//void Flight::updateFlight(int flightNumber) {
//    // Find the flight
//    Flight* flightToUpdate = flightBST.find(flightNumber);
//    if (!flightToUpdate) {
//        cout << "Flight with number " << flightNumber << " does not exist!\n";
//        return;
//    }
//
//    cout << "Updating Flight No: " << flightNumber << "\n";
//    cout << "Current Information:\n";
//    flightToUpdate->displayInfo();
//
//    string temp;
//    bool validInput = false;
//
//    // Update Departure
//    do {
//        cout << "Current Departure: " << flightToUpdate->from << "\n";
//        cout << "Enter new Departure (leave blank to keep current): ";
//        getline(cin, temp);
//
//        if (temp.empty()) {
//            break; // Keep current value
//        }
//
//        if (temp.length() > 20 || !checkString(temp)) {
//            cout << "Please insert a valid Departure city!\n";
//            continue;
//        }
//        flightToUpdate->from = temp;
//        break;
//    } while (true);
//
//    // Update Destination
//    do {
//        cout << "Current Destination: " << flightToUpdate->to << "\n";
//        cout << "Enter new Destination (leave blank to keep current): ";
//        getline(cin, temp);
//
//        if (temp.empty()) {
//            break; // Keep current value
//        }
//
//        if (temp.length() > 20 || !checkString(temp) || temp == flightToUpdate->from) {
//            cout << "Please insert a valid Destination city!\n";
//            continue;
//        }
//        flightToUpdate->to = temp;
//        break;
//    } while (true);
//
//    // Update Boarding time
//    do {
//        cout << "Current Boarding Time: "
//            << setw(2) << setfill('0') << flightToUpdate->t_leave.hour << ":"
//            << setw(2) << setfill('0') << flightToUpdate->t_leave.min << "\n";
//        cout << "Enter new Boarding time (HH:MM format, leave blank to keep current): ";
//        getline(cin, temp);
//
//        if (temp.empty()) {
//            break; // Keep current value
//        }
//
//        if (!checkTime(temp)) {
//            cout << "Please insert a valid boarding time (e.g. 19:40)!\n";
//            continue;
//        }
//
//        size_t colon = temp.find(':');
//        Time newTime;
//        newTime.hour = stoi(temp.substr(0, colon));
//        newTime.min = stoi(temp.substr(colon + 1));
//
//        // Check if new boarding time is before current arrival time
//        if (checkTime2(newTime, flightToUpdate->t_arrive)) {
//            cout << "Boarding time must be before arrival time!\n";
//            continue;
//        }
//
//        flightToUpdate->t_leave = newTime;
//        break;
//    } while (true);
//
//    // Update Arrival time
//    do {
//        cout << "Current Arrival Time: "
//            << setw(2) << setfill('0') << flightToUpdate->t_arrive.hour << ":"
//            << setw(2) << setfill('0') << flightToUpdate->t_arrive.min << "\n";
//        cout << "Enter new Arrival time (HH:MM format, leave blank to keep current): ";
//        getline(cin, temp);
//
//        if (temp.empty()) {
//            break; // Keep current value
//        }
//
//        if (!checkTime(temp)) {
//            cout << "Please insert a valid arriving time (e.g. 21:40)!\n";
//            continue;
//        }
//
//        size_t colon = temp.find(':');
//        Time newTime;
//        newTime.hour = stoi(temp.substr(0, colon));
//        newTime.min = stoi(temp.substr(colon + 1));
//
//        // Check if new arrival time is after current boarding time
//        if (checkTime2(flightToUpdate->t_leave, newTime)) {
//            cout << "Arrival time must be after departure time!\n";
//            continue;
//        }
//
//        flightToUpdate->t_arrive = newTime;
//        break;
//    } while (true);
//
//    // Update Ticket price
//    do {
//        cout << "Current Ticket Price: " << flightToUpdate->cost << "\n";
//        cout << "Enter new Ticket price (leave blank to keep current): ";
//        getline(cin, temp);
//
//        if (temp.empty()) {
//            break; // Keep current value
//        }
//
//        if (!checkNumber(temp)) {
//            cout << "Please insert a valid ticket price!\n";
//            continue;
//        }
//        flightToUpdate->cost = stoi(temp);
//        break;
//    } while (true);
//
//    // Update Aeroplane type
//    do {
//        cout << "Current Aeroplane Type: " << flightToUpdate->plane_type << "\n";
//        cout << "Enter new Aeroplane type (leave blank to keep current): ";
//        getline(cin, temp);
//
//        if (temp.empty()) {
//            break; // Keep current value
//        }
//
//        if (temp.empty()) {
//            cout << "Please insert a valid Aeroplane type!\n";
//            continue;
//        }
//        flightToUpdate->plane_type = temp;
//        break;
//    } while (true);
//
//    // Update Number of seats
//    do {
//        cout << "Current Number of Seats: " << flightToUpdate->seats << "\n";
//        cout << "Enter new Number of seats (leave blank to keep current): ";
//        getline(cin, temp);
//
//        if (temp.empty()) {
//            break; // Keep current value
//        }
//
//        if (!checkNumber(temp)) {
//            cout << "Please insert a valid number of seats!\n";
//            continue;
//        }
//
//        int newSeats = stoi(temp);
//        if (newSeats < flightToUpdate->booked_seats) {
//            cout << "Cannot reduce seats below currently booked seats ("
//                << flightToUpdate->booked_seats << ")!\n";
//            continue;
//        }
//        flightToUpdate->seats = newSeats;
//        break;
//    } while (true);
//
//    // Update Weather condition
//    do {
//        cout << "Current Weather Condition: " << flightToUpdate->weather << "\n";
//        cout << "Enter new Weather condition (leave blank to keep current): ";
//        getline(cin, temp);
//
//        if (temp.empty()) {
//            break; // Keep current value
//        }
//
//        if (temp.empty()) {
//            cout << "Please enter a valid weather condition!\n";
//            continue;
//        }
//        flightToUpdate->weather = temp;
//        break;
//    } while (true);
//
//    // Update Day of week
//    do {
//        cout << "Current Day of Week: " << flightToUpdate->dayOfWeek << "\n";
//        cout << "Enter new Day of week (leave blank to keep current): ";
//        getline(cin, temp);
//
//        if (temp.empty()) {
//            break; // Keep current value
//        }
//
//        if (temp.empty()) {
//            cout << "Please enter a valid day of week!\n";
//            continue;
//        }
//        flightToUpdate->dayOfWeek = temp;
//        break;
//    } while (true);
//
//    // Save changes to file
//    Flight::saveToFile();
//    cout << "\nFlight " << flightNumber << " has been successfully updated.\n";
//}
// Menu member functions implementation
void Menu::displayMenu() {
    int selection;
    string temp;

    Flight::loadFromFile();
    Customer::loadFromFile();
    Queue::loadFromFile();

    do {
        cout << "-----------------------------------------------";
        cout << "\n\t AIRLINE RESERVATION SYSTEM \n\n";
        cout << "Please select an option from the menu below: \n";
        cout << "1. Add new flights \n";
        cout << "2. Delete flights \n";
        cout << "3. Display flight schedule \n";
        cout << "4. Display flight details \n";
        cout << "5. Display passenger Customeral info \n";
        cout << "6. Book reservation \n";
        cout << "7. Cancel reservation \n";
        cout << "8. update flight\n" ;
        cout << "9. Manage waiting queue \n";
        cout << "10. Exit \n";
        cout << "-----------------------------------------------" << endl;

        cout << "Enter your choice (1-9): ";
        cin >> temp;

        while (!checkNumber(temp) || stoi(temp) < 1 || stoi(temp) > 10) {
            cout << "Please enter a valid number between 1 and 10!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> temp;
        }
        cout << endl;

        selection = stoi(temp);
        select(selection);
    } while (true);
}

void Menu::select(int selection) {
    Flight f;
    Customer p;
    string temp;

    try {
        switch (selection) {
        case 1:
            f.addFlight();
            break;
        case 2:
            if (!flist.empty()) {
                cout << "Enter the Flight Number you want to delete: ";
                cin >> temp;
                cout << endl;

                while (!checkNumber(temp)) {
                    cout << "Flight Number must be a number!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> temp;
                    cout << endl;
                }

                Flight::deleteFlight(stoi(temp));
            }
            else {
                cout << "There are no flights to delete\n";
            }
            break;
        case 3:
            if (!flist.empty()) {
                Flight::displaySchedule();
            }
            else {
                cout << "There are no scheduled flights!\n";
            }
            break;
        case 4:
            if (!flist.empty()) {
                cout << "Please enter flight number: ";
                cin >> temp;
                cout << endl;

                while (!checkNumber(temp)) {
                    cout << "Flight number must be a number!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> temp;
                    cout << endl;
                }

                Flight::displayFlightInfo(stoi(temp));
            }
            else {
                cout << "There are no scheduled flights!\n";
            }
            break;
        case 5:
            if (!customerBST.getAllCustomers().empty()) {
                cout << "Please enter passport number: ";
                cin >> temp;

                while (!Customer::displayCustomerInfo(stoi(temp))) {
                    cout << "Wrong passport number!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> temp;
                    cout << endl;
                }
            }
            else {
                cout << "There are no registered clients at the moment!\n";
            }
            break;
        case 6:
            p.book();
            break;
        case 7:
            p.cancel();
            break;
        case 8:  
            if (!flist.empty()) {
                cout << "Enter flight number to update: ";
                cin >> temp;
                cout << endl;

                while (!checkNumber(temp)) {
                    cout << "Flight number must be a number!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> temp;
                    cout << endl;
                }

                Flight::updateFlight(stoi(temp));
            }
            else {
                cout << "There are no scheduled flights!\n";
            }
            break;
        case 9:
            if (!flist.empty()) {
                cout << "Enter flight number to manage queue: ";
                cin >> temp;
                cout << endl;

                while (!checkNumber(temp)) {
                    cout << "Flight number must be a number!\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> temp;
                    cout << endl;
                }

                Customer customer;
                customer.manageQueue(stoi(temp));
            }
            else {
                cout << "There are no scheduled flights!\n";
            }
            break;
        case 10:
            Menu::exit_prog();
            break;
        default:
            cout << "Invalid selection \n";
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void Menu::exit_prog() {
    Flight::saveToFile();
    Customer::saveToFile();
    Queue::saveToFile();

    DoublyLinkedList<Customer*> customers = customerBST.getAllCustomers();
    for (ListNode<Customer*>* node = customers.begin(); node != customers.end(); node = node->next) {
        delete node->data;
    }

    cout << "Thank you for using our system! \n";
    exit(0);
}

int main() {
    try {
        Menu::displayMenu();
    }
    catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1; 
    }
    return 0;
}
