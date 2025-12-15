#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <functional>
#include <algorithm>
#include <vector>
#include <random>
#include <limits>
#include <stdexcept>
#include <memory>
#include <chrono>
#include <climits>

// ===================== Класс Date =====================
class Date {
private:
    int day;
    int month;
    int year;

    bool isValidDate(int d, int m, int y) const {
        if (y < 1900 || y > 2100) return false;
        if (m < 1 || m > 12) return false;
        
        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        if (m == 2) {
            bool isLeap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
            if (isLeap) daysInMonth[1] = 29;
        }
        
        return d >= 1 && d <= daysInMonth[m - 1];
    }

public:
    Date() : day(1), month(1), year(2000) {}
    
    Date(int d, int m, int y) {
        if (isValidDate(d, m, y)) {
            day = d;
            month = m;
            year = y;
        } else {
            day = 1;
            month = 1;
            year = 2000;
        }
    }
    
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
    
    void setDay(int d) { if (d >= 1 && d <= 31) day = d; }
    void setMonth(int m) { if (m >= 1 && m <= 12) month = m; }
    void setYear(int y) { if (y >= 1900 && y <= 2100) year = y; }
    
    // Операторы сравнения
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
    
    bool operator>(const Date& other) const { return other < *this; }
    
    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }
    
    bool operator!=(const Date& other) const { return !(*this == other); }
    
    bool operator<=(const Date& other) const {
        return *this < other || *this == other;
    }
    
    bool operator>=(const Date& other) const {
        return *this > other || *this == other;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Date& date) {
        os << std::setw(2) << std::setfill('0') << date.day << "."
           << std::setw(2) << std::setfill('0') << date.month << "."
           << date.year;
        return os;
    }
    
    friend std::istream& operator>>(std::istream& is, Date& date) {
        std::string input;
        is >> input;
        std::istringstream iss(input);
        char delimiter;
        int d, m, y;
        
        if (iss >> d >> delimiter >> m >> delimiter >> y) {
            if (date.isValidDate(d, m, y)) {
                date.day = d;
                date.month = m;
                date.year = y;
            }
        }
        return is;
    }
    
    void serialize(std::ostream& os) const {
        os.write(reinterpret_cast<const char*>(&day), sizeof(day));
        os.write(reinterpret_cast<const char*>(&month), sizeof(month));
        os.write(reinterpret_cast<const char*>(&year), sizeof(year));
    }
    
    void deserialize(std::istream& is) {
        is.read(reinterpret_cast<char*>(&day), sizeof(day));
        is.read(reinterpret_cast<char*>(&month), sizeof(month));
        is.read(reinterpret_cast<char*>(&year), sizeof(year));
    }
    
    static Date getCurrentDate() {
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        return Date(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    }
    
    int getAge() const {
        Date current = getCurrentDate();
        int age = current.year - year;
        if (current.month < month || (current.month == month && current.day < day)) {
            age--;
        }
        return age;
    }
};

// ===================== Класс Student =====================
class Student {
private:
    std::string lastName;
    Date birthDate;
    Date admissionDate;
    Date expulsionDate;
    std::string address;
    std::string group;
    
public:
    Student() : lastName(""), birthDate(1, 1, 2000), 
                admissionDate(1, 9, 2020), expulsionDate(30, 6, 2024),
                address(""), group("") {}
    
    Student(const std::string& ln, const Date& bd, const Date& ad, 
            const Date& ed, const std::string& addr, const std::string& gr)
        : lastName(ln), birthDate(bd), admissionDate(ad), 
          expulsionDate(ed), address(addr), group(gr) {}
    
    std::string getLastName() const { return lastName; }
    Date getBirthDate() const { return birthDate; }
    Date getAdmissionDate() const { return admissionDate; }
    Date getExpulsionDate() const { return expulsionDate; }
    std::string getAddress() const { return address; }
    std::string getGroup() const { return group; }
    
    void setLastName(const std::string& ln) { lastName = ln; }
    void setBirthDate(const Date& bd) { birthDate = bd; }
    void setAdmissionDate(const Date& ad) { admissionDate = ad; }
    void setExpulsionDate(const Date& ed) { expulsionDate = ed; }
    void setAddress(const std::string& addr) { address = addr; }
    void setGroup(const std::string& gr) { group = gr; }
    
    bool operator<(const Student& other) const { return lastName < other.lastName; }
    bool operator>(const Student& other) const { return lastName > other.lastName; }
    bool operator==(const Student& other) const {
        return lastName == other.lastName && 
               birthDate == other.birthDate &&
               group == other.group;
    }
    
    bool compareByBirthDate(const Student& other) const { return birthDate < other.birthDate; }
    bool compareByAdmissionDate(const Student& other) const { return admissionDate < other.admissionDate; }
    bool compareByGroup(const Student& other) const {
        if (group != other.group) return group < other.group;
        return lastName < other.lastName;
    }
    
    void serialize(std::ostream& os) const {
        size_t len = lastName.length();
        os.write(reinterpret_cast<const char*>(&len), sizeof(len));
        os.write(lastName.c_str(), len);
        birthDate.serialize(os);
        admissionDate.serialize(os);
        expulsionDate.serialize(os);
        
        len = address.length();
        os.write(reinterpret_cast<const char*>(&len), sizeof(len));
        os.write(address.c_str(), len);
        
        len = group.length();
        os.write(reinterpret_cast<const char*>(&len), sizeof(len));
        os.write(group.c_str(), len);
    }
    
    void deserialize(std::istream& is) {
        size_t len;
        
        is.read(reinterpret_cast<char*>(&len), sizeof(len));
        char* buffer = new char[len + 1];
        is.read(buffer, len);
        buffer[len] = '\0';
        lastName = buffer;
        delete[] buffer;
        
        birthDate.deserialize(is);
        admissionDate.deserialize(is);
        expulsionDate.deserialize(is);
        
        is.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        is.read(buffer, len);
        buffer[len] = '\0';
        address = buffer;
        delete[] buffer;
        
        is.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        is.read(buffer, len);
        buffer[len] = '\0';
        group = buffer;
        delete[] buffer;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        os << "Фамилия: " << student.lastName << "\n"
           << "Дата рождения: " << student.birthDate 
           << " (возраст: " << student.birthDate.getAge() << " лет)\n"
           << "Дата поступления: " << student.admissionDate << "\n"
           << "Дата отчисления: " << student.expulsionDate << "\n"
           << "Адрес: " << student.address << "\n"
           << "Группа: " << student.group << "\n";
        
        Date current = Date::getCurrentDate();
        int course = current.getYear() - student.admissionDate.getYear();
        if (current.getMonth() < 9) course--;
        if (course < 1) course = 1;
        if (course > 5) course = 5;
        
        os << "Курс: " << course << "\n"
           << "Статус: " << (student.isCurrentlyStudying() ? "обучается" : "отчислен") << "\n"
           << "--------------------------------------------------\n";
        return os;
    }
    
    bool isCurrentlyStudying() const {
        Date current = Date::getCurrentDate();
        return current >= admissionDate && current <= expulsionDate;
    }
};

// ===================== Шаблонный класс Node =====================
template <typename T>
class Node {
public:
    T data;
    Node<T>* next;
    
    Node(const T& data) : data(data), next(nullptr) {}
};

// ===================== Абстрактный класс Container =====================
template <typename T>
class Container {
public:
    virtual ~Container() {}
    virtual void add(const T& item) = 0;
    virtual void insert(int index, const T& item) = 0;
    virtual void remove(int index) = 0;
    virtual void remove(const T& item) = 0;
    virtual T get(int index) const = 0;
    virtual void set(int index, const T& item) = 0;
    virtual int getSize() const = 0;
    virtual bool isEmpty() const = 0;
    virtual void clear() = 0;
    virtual int find(const T& item) const = 0;
    virtual bool contains(const T& item) const = 0;
    virtual void sort(bool ascending = true) = 0;
    virtual void sort(std::function<bool(const T&, const T&)> comparator) = 0;
    virtual bool saveToFile(const std::string& filename) const = 0;
    virtual bool loadFromFile(const std::string& filename) = 0;
    virtual void print() const = 0;
};

// ===================== Класс LinkedList =====================
template <typename T>
class LinkedList : public Container<T> {
private:
    Node<T>* head;
    int size;
    
    Node<T>* getNode(int index) const {
        if (index < 0 || index >= size) return nullptr;
        Node<T>* current = head;
        for (int i = 0; i < index; i++) current = current->next;
        return current;
    }
    
    void swapNodes(int i, int j) {
        if (i < 0 || i >= size || j < 0 || j >= size || i == j) return;
        T temp = get(i);
        set(i, get(j));
        set(j, temp);
    }
    
    void quickSort(int left, int right, std::function<bool(const T&, const T&)> comparator) {
        if (left >= right) return;
        T pivot = get((left + right) / 2);
        int i = left, j = right;
        
        while (i <= j) {
            while (comparator(get(i), pivot)) i++;
            while (comparator(pivot, get(j))) j--;
            if (i <= j) {
                swapNodes(i, j);
                i++; j--;
            }
        }
        
        if (left < j) quickSort(left, j, comparator);
        if (i < right) quickSort(i, right, comparator);
    }
    
public:
    LinkedList() : head(nullptr), size(0) {}
    
    LinkedList(const LinkedList<T>& other) : head(nullptr), size(0) {
        Node<T>* current = other.head;
        while (current) {
            add(current->data);
            current = current->next;
        }
    }
    
    ~LinkedList() { clear(); }
    
    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            clear();
            Node<T>* current = other.head;
            while (current) {
                add(current->data);
                current = current->next;
            }
        }
        return *this;
    }
    
    virtual void add(const T& item) override {
        Node<T>* newNode = new Node<T>(item);
        if (!head) {
            head = newNode;
        } else {
            Node<T>* current = head;
            while (current->next) current = current->next;
            current->next = newNode;
        }
        size++;
    }
    
    virtual void insert(int index, const T& item) override {
        if (index < 0 || index > size) throw std::out_of_range("Index out of range");
        
        if (index == 0) {
            Node<T>* newNode = new Node<T>(item);
            newNode->next = head;
            head = newNode;
        } else if (index == size) {
            add(item);
            return;
        } else {
            Node<T>* prev = getNode(index - 1);
            Node<T>* newNode = new Node<T>(item);
            newNode->next = prev->next;
            prev->next = newNode;
        }
        size++;
    }
    
    virtual void remove(int index) override {
        if (index < 0 || index >= size) throw std::out_of_range("Index out of range");
        
        if (index == 0) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        } else {
            Node<T>* prev = getNode(index - 1);
            Node<T>* toDelete = prev->next;
            prev->next = toDelete->next;
            delete toDelete;
        }
        size--;
    }
    
    virtual void remove(const T& item) override {
        int index = find(item);
        if (index != -1) remove(index);
    }
    
    virtual T get(int index) const override {
        Node<T>* node = getNode(index);
        if (!node) throw std::out_of_range("Index out of range");
        return node->data;
    }
    
    virtual void set(int index, const T& item) override {
        Node<T>* node = getNode(index);
        if (!node) throw std::out_of_range("Index out of range");
        node->data = item;
    }
    
    virtual int getSize() const override { return size; }
    virtual bool isEmpty() const override { return size == 0; }
    
    virtual void clear() override {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }
    
    virtual int find(const T& item) const override {
        Node<T>* current = head;
        int index = 0;
        while (current) {
            if (current->data == item) return index;
            current = current->next;
            index++;
        }
        return -1;
    }
    
    virtual bool contains(const T& item) const override {
        return find(item) != -1;
    }
    
    virtual void sort(bool ascending = true) override {
        sort([ascending](const T& a, const T& b) {
            return ascending ? a < b : b < a;
        });
    }
    
    virtual void sort(std::function<bool(const T&, const T&)> comparator) override {
        if (size <= 1) return;
        quickSort(0, size - 1, comparator);
    }
    
    virtual bool saveToFile(const std::string& filename) const override {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) return false;
        
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        Node<T>* current = head;
        while (current) {
            current->data.serialize(file);
            current = current->next;
        }
        
        file.close();
        return true;
    }
    
    virtual bool loadFromFile(const std::string& filename) override {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) return false;
        
        clear();
        int fileSize;
        file.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
        
        for (int i = 0; i < fileSize; i++) {
            T item;
            item.deserialize(file);
            add(item);
        }
        
        file.close();
        return true;
    }
    
    virtual void print() const override {
        if (isEmpty()) {
            std::cout << "Список пуст.\n";
            return;
        }
        
        Node<T>* current = head;
        int counter = 1;
        while (current) {
            std::cout << "[" << counter << "] " << current->data << "\n";
            current = current->next;
            counter++;
        }
    }
    
    void addSorted(const T& item, bool ascending = true) {
        if (isEmpty()) {
            add(item);
            return;
        }
        
        int index = 0;
        Node<T>* current = head;
        while (current) {
            if ((ascending && item < current->data) || 
                (!ascending && current->data < item)) {
                insert(index, item);
                return;
            }
            current = current->next;
            index++;
        }
        add(item);
    }
    
    std::vector<T> findByPredicate(std::function<bool(const T&)> predicate) const {
        std::vector<T> results;
        Node<T>* current = head;
        while (current) {
            if (predicate(current->data)) results.push_back(current->data);
            current = current->next;
        }
        return results;
    }
    
    T& operator[](int index) {
        Node<T>* node = getNode(index);
        if (!node) throw std::out_of_range("Index out of range");
        return node->data;
    }
    
    const T& operator[](int index) const {
        Node<T>* node = getNode(index);
        if (!node) throw std::out_of_range("Index out of range");
        return node->data;
    }
};

// ===================== Класс StudentManager =====================
class StudentManager {
private:
    LinkedList<Student> students;
    
public:
    void addStudent(const Student& student) { students.add(student); }
    void insertStudent(int index, const Student& student) { students.insert(index, student); }
    void removeStudent(int index) { students.remove(index); }
    
    void removeStudentByLastName(const std::string& lastName) {
        for (int i = 0; i < students.getSize(); i++) {
            if (students.get(i).getLastName() == lastName) {
                students.remove(i);
                return;
            }
        }
    }
    
    Student getStudent(int index) const { return students.get(index); }
    void updateStudent(int index, const Student& student) { students.set(index, student); }
    int getStudentCount() const { return students.getSize(); }
    bool isEmpty() const { return students.isEmpty(); }
    void clearAll() { students.clear(); }
    
    std::vector<Student> searchByGroup(const std::string& group) const {
        return students.findByPredicate([&group](const Student& s) {
            return s.getGroup() == group;
        });
    }
    
    std::vector<Student> searchByLastName(const std::string& lastName) const {
        return students.findByPredicate([&lastName](const Student& s) {
            return s.getLastName() == lastName;
        });
    }
    
    std::vector<Student> searchByLastNamePattern(const std::string& pattern) const {
        return students.findByPredicate([&pattern](const Student& s) {
            return s.getLastName().find(pattern) != std::string::npos;
        });
    }
    
    std::vector<Student> searchCurrentlyStudying() const {
        return students.findByPredicate([](const Student& s) {
            return s.isCurrentlyStudying();
        });
    }
    
    void sortByLastName(bool ascending = true) {
        students.sort([ascending](const Student& a, const Student& b) {
            return ascending ? a < b : b < a;
        });
    }
    
    void sortByBirthDate(bool ascending = true) {
        students.sort([ascending](const Student& a, const Student& b) {
            return ascending ? a.compareByBirthDate(b) : b.compareByBirthDate(a);
        });
    }
    
    void sortByAdmissionDate(bool ascending = true) {
        students.sort([ascending](const Student& a, const Student& b) {
            return ascending ? a.compareByAdmissionDate(b) : b.compareByAdmissionDate(a);
        });
    }
    
    void sortByGroup(bool ascending = true) {
        students.sort([ascending](const Student& a, const Student& b) {
            return ascending ? a.compareByGroup(b) : b.compareByGroup(a);
        });
    }
    
    void addStudentSorted(const Student& student, bool ascending = true) {
        students.addSorted(student, ascending);
    }
    
    void printStatistics() const {
        if (students.isEmpty()) {
            std::cout << "Нет данных для статистики.\n";
            return;
        }
        
        int total = students.getSize();
        int currentlyStudying = searchCurrentlyStudying().size();
        
        Student oldest = students.get(0);
        Student youngest = students.get(0);
        for (int i = 1; i < students.getSize(); i++) {
            Student current = students.get(i);
            if (current.getBirthDate() < oldest.getBirthDate()) oldest = current;
            if (current.getBirthDate() > youngest.getBirthDate()) youngest = current;
        }
        
        std::cout << "=== Статистика ===\n"
                  << "Всего студентов: " << total << "\n"
                  << "Обучается сейчас: " << currentlyStudying << "\n"
                  << "Самый старший: " << oldest.getLastName() 
                  << " (" << oldest.getBirthDate().getAge() << " лет)\n"
                  << "Самый младший: " << youngest.getLastName() 
                  << " (" << youngest.getBirthDate().getAge() << " лет)\n"
                  << "==================\n";
    }
    
    void editByGroupAndPattern(const std::string& group, const std::string& pattern,
                               const Student& newData) {
        for (int i = 0; i < students.getSize(); i++) {
            Student& student = students[i];
            if (student.getGroup() == group && 
                student.getLastName().find(pattern) != std::string::npos) {
                student = newData;
            }
        }
    }
    
    void removeByGroup(const std::string& group) {
        for (int i = students.getSize() - 1; i >= 0; i--) {
            if (students.get(i).getGroup() == group) {
                students.remove(i);
            }
        }
    }
    
    bool saveToFile(const std::string& filename) const { return students.saveToFile(filename); }
    bool loadFromFile(const std::string& filename) { return students.loadFromFile(filename); }
    
    void printAllStudents() const {
        std::cout << "=== Список студентов (" << students.getSize() << " шт.) ===\n";
        students.print();
    }
    
    void generateTestData(int count) {
        std::vector<std::string> lastNames = {"Иванов", "Петров", "Сидоров", "Кузнецов", 
                                             "Смирнов", "Попов", "Васильев", "Павлов"};
        std::vector<std::string> groups = {"ИВТ-101", "ИВТ-102", "ПМИ-201", "ПМИ-202", "ФИИТ-301"};
        std::vector<std::string> addresses = {"ул. Ленина, 10", "ул. Советская, 25", 
                                              "ул. Мира, 15", "ул. Пушкина, 7"};
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> lastNameDist(0, lastNames.size() - 1);
        std::uniform_int_distribution<> groupDist(0, groups.size() - 1);
        std::uniform_int_distribution<> addressDist(0, addresses.size() - 1);
        std::uniform_int_distribution<> yearDist(1995, 2005);
        std::uniform_int_distribution<> monthDist(1, 12);
        std::uniform_int_distribution<> dayDist(1, 28);
        
        for (int i = 0; i < count; i++) {
            std::string lastName = lastNames[lastNameDist(gen)];
            int birthYear = yearDist(gen);
            Date birthDate(dayDist(gen), monthDist(gen), birthYear);
            Date admissionDate(1, 9, birthYear + 18);
            Date expulsionDate(30, 6, birthYear + 22);
            std::string address = addresses[addressDist(gen)];
            std::string group = groups[groupDist(gen)];
            
            students.add(Student(lastName, birthDate, admissionDate, 
                                expulsionDate, address, group));
        }
    }
    
    void testPerformance() {
        std::cout << "=== Тестирование производительности ===\n";
        
        if (students.isEmpty()) {
            std::cout << "Генерирую тестовые данные...\n";
            generateTestData(1000);
        }
        
        int n = students.getSize();
        std::cout << "Элементов: " << n << "\n";
        
        auto start = std::chrono::high_resolution_clock::now();
        sortByLastName();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Сортировка: " << duration.count() << " мс\n";
        
        start = std::chrono::high_resolution_clock::now();
        auto results = searchByGroup("ИВТ-101");
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Поиск: " << duration.count() << " мс (найдено: " 
                  << results.size() << ")\n";
        
        start = std::chrono::high_resolution_clock::now();
        Student testStudent("Тестовый", Date(1, 1, 2000), Date(1, 9, 2020),
                           Date(30, 6, 2024), "ул. Тестовая, 1", "ТЕСТ-001");
        students.add(testStudent);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Добавление: " << duration.count() << " мс\n";
        
        std::cout << "=======================================\n";
    }
};

// ===================== Класс Menu =====================
class Menu {
private:
    StudentManager manager;
    
    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    int getIntInput(const std::string& prompt, int min = INT_MIN, int max = INT_MAX) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                if (value >= min && value <= max) {
                    clearInput();
                    return value;
                }
            }
            std::cout << "Ошибка! Введите число от " << min << " до " << max << ".\n";
            clearInput();
        }
    }
    
    std::string getStringInput(const std::string& prompt) {
        std::string value;
        std::cout << prompt;
        std::getline(std::cin, value);
        return value;
    }
    
    Student inputStudent() {
        std::cout << "=== Ввод данных студента ===\n";
        std::string lastName = getStringInput("Фамилия: ");
        
        std::cout << "Дата рождения (ДД ММ ГГГГ): ";
        int d, m, y;
        std::cin >> d >> m >> y;
        clearInput();
        Date birthDate(d, m, y);
        
        std::cout << "Дата поступления (ДД ММ ГГГГ): ";
        std::cin >> d >> m >> y;
        clearInput();
        Date admissionDate(d, m, y);
        
        std::cout << "Дата отчисления (ДД ММ ГГГГ): ";
        std::cin >> d >> m >> y;
        clearInput();
        Date expulsionDate(d, m, y);
        
        std::string address = getStringInput("Адрес: ");
        std::string group = getStringInput("Группа: ");
        
        return Student(lastName, birthDate, admissionDate, expulsionDate, address, group);
    }
    
    void printStudentList() {
        if (manager.isEmpty()) {
            std::cout << "Список студентов пуст.\n";
            return;
        }
        
        for (int i = 0; i < manager.getStudentCount(); i++) {
            std::cout << "[" << i + 1 << "] " << manager.getStudent(i).getLastName() 
                      << " (" << manager.getStudent(i).getGroup() << ")\n";
        }
    }
    
    void addStudentMenu() {
        Student student = inputStudent();
        manager.addStudent(student);
        std::cout << "Студент добавлен!\n";
    }
    
    void insertStudentMenu() {
        if (manager.isEmpty()) {
            std::cout << "Список пуст.\n";
            return;
        }
        
        printStudentList();
        int index = getIntInput("Позиция для вставки (1-" + 
                               std::to_string(manager.getStudentCount() + 1) + "): ", 
                               1, manager.getStudentCount() + 1);
        
        Student student = inputStudent();
        manager.insertStudent(index - 1, student);
        std::cout << "Студент вставлен!\n";
    }
    
    void removeStudentMenu() {
        if (manager.isEmpty()) {
            std::cout << "Список пуст.\n";
            return;
        }
        
        int choice = getIntInput("1. Удалить по номеру\n2. Удалить по фамилии\n3. Удалить по группе\nВыберите: ", 1, 3);
        
        switch (choice) {
            case 1: {
                printStudentList();
                int index = getIntInput("Номер для удаления: ", 1, manager.getStudentCount());
                manager.removeStudent(index - 1);
                std::cout << "Удалено!\n";
                break;
            }
            case 2: {
                std::string lastName = getStringInput("Фамилия: ");
                manager.removeStudentByLastName(lastName);
                std::cout << "Удалено!\n";
                break;
            }
            case 3: {
                std::string group = getStringInput("Группа: ");
                manager.removeByGroup(group);
                std::cout << "Удалено!\n";
                break;
            }
        }
    }
    
    void editStudentMenu() {
        if (manager.isEmpty()) {
            std::cout << "Список пуст.\n";
            return;
        }
        
        printStudentList();
        int index = getIntInput("Номер студента для редактирования: ", 
                               1, manager.getStudentCount());
        
        std::cout << "Текущие данные:\n" << manager.getStudent(index - 1) << "\n";
        Student newStudent = inputStudent();
        manager.updateStudent(index - 1, newStudent);
        std::cout << "Данные обновлены!\n";
    }
    
    void searchMenu() {
        int choice = getIntInput("1. По группе\n2. По фамилии\n3. По шаблону\n4. Обучающиеся сейчас\nВыберите: ", 1, 4);
        
        std::vector<Student> results;
        
        switch (choice) {
            case 1: {
                std::string group = getStringInput("Группа: ");
                results = manager.searchByGroup(group);
                break;
            }
            case 2: {
                std::string lastName = getStringInput("Фамилия: ");
                results = manager.searchByLastName(lastName);
                break;
            }
            case 3: {
                std::string pattern = getStringInput("Шаблон фамилии: ");
                results = manager.searchByLastNamePattern(pattern);
                break;
            }
            case 4: {
                results = manager.searchCurrentlyStudying();
                break;
            }
        }
        
        if (results.empty()) {
            std::cout << "Не найдено.\n";
            return;
        }
        
        std::cout << "Найдено " << results.size() << " студентов:\n";
        for (const auto& student : results) {
            std::cout << student << "\n";
        }
    }
    
    void sortMenu() {
        int choice = getIntInput("1. По фамилии ▲\n2. По фамилии ▼\n3. По дате рождения\n4. По группе\nВыберите: ", 1, 4);
        
        switch (choice) {
            case 1: manager.sortByLastName(true); break;
            case 2: manager.sortByLastName(false); break;
            case 3: {
                bool asc = getIntInput("1. ▲\n2. ▼\nВыберите: ", 1, 2) == 1;
                manager.sortByBirthDate(asc);
                break;
            }
            case 4: {
                bool asc = getIntInput("1. ▲\n2. ▼\nВыберите: ", 1, 2) == 1;
                manager.sortByGroup(asc);
                break;
            }
        }
        
        std::cout << "Сортировка завершена!\n";
    }
    
    void fileMenu() {
        int choice = getIntInput("1. Сохранить\n2. Загрузить\nВыберите: ", 1, 2);
        std::string filename = getStringInput("Имя файла: ");
        
        if (choice == 1) {
            if (manager.saveToFile(filename)) {
                std::cout << "Сохранено!\n";
            } else {
                std::cout << "Ошибка сохранения!\n";
            }
        } else {
            if (manager.loadFromFile(filename)) {
                std::cout << "Загружено!\n";
            } else {
                std::cout << "Ошибка загрузки!\n";
            }
        }
    }
    
    void addSortedMenu() {
        Student student = inputStudent();
        int order = getIntInput("1. По возрастанию\n2. По убыванию\nВыберите: ", 1, 2);
        manager.addStudentSorted(student, order == 1);
        std::cout << "Студент добавлен с сохранением порядка!\n";
    }
    
    void groupEditMenu() {
        std::string group = getStringInput("Введите группу: ");
        std::string pattern = getStringInput("Введите шаблон фамилии: ");
        
        auto groupStudents = manager.searchByGroup(group);
        auto patternStudents = manager.searchByLastNamePattern(pattern);
        
        std::cout << "Найдено студентов в группе " << group << ": " << groupStudents.size() << "\n";
        std::cout << "Найдено студентов по шаблону '" << pattern << "': " << patternStudents.size() << "\n";
        
        std::cout << "Введите новые данные для редактирования:\n";
        Student newData = inputStudent();
        
        manager.editByGroupAndPattern(group, pattern, newData);
        std::cout << "Редактирование завершено!\n";
    }
    
public:
    void run() {
        std::cout << "=== Система управления студентами ===\n";
        
        while (true) {
            std::cout << "\n=== МЕНЮ ===\n";
            std::cout << "Студентов: " << manager.getStudentCount() << "\n";
            std::cout << "1. Добавить студента\n";
            std::cout << "2. Вставить студента\n";
            std::cout << "3. Удалить студента\n";
            std::cout << "4. Редактировать студента\n";
            std::cout << "5. Показать всех\n";
            std::cout << "6. Поиск студентов\n";
            std::cout << "7. Сортировка студентов\n";
            std::cout << "8. Статистика\n";
            std::cout << "9. Работа с файлами\n";
            std::cout << "10. Добавить с сохранением порядка\n";
            std::cout << "11. Редактировать по группе и шаблону\n";
            std::cout << "12. Тестирование производительности\n";
            std::cout << "13. Генерация тестовых данных\n";
            std::cout << "0. Выход\n";
            
            int choice = getIntInput("Выберите действие: ", 0, 13);
            
            switch (choice) {
                case 0:
                    std::cout << "Выход из программы...\n";
                    return;
                case 1:
                    addStudentMenu();
                    break;
                case 2:
                    insertStudentMenu();
                    break;
                case 3:
                    removeStudentMenu();
                    break;
                case 4:
                    editStudentMenu();
                    break;
                case 5:
                    manager.printAllStudents();
                    break;
                case 6:
                    searchMenu();
                    break;
                case 7:
                    sortMenu();
                    break;
                case 8:
                    manager.printStatistics();
                    break;
                case 9:
                    fileMenu();
                    break;
                case 10:
                    addSortedMenu();
                    break;
                case 11:
                    groupEditMenu();
                    break;
                case 12:
                    manager.testPerformance();
                    break;
                case 13: {
                    int count = getIntInput("Сколько записей сгенерировать? ", 1, 10000);
                    manager.generateTestData(count);
                    std::cout << "Сгенерировано " << count << " записей\n";
                    break;
                }
                default:
                    std::cout << "Неверный выбор!\n";
            }
        }
    }
};

// ===================== Main функция =====================
int main() {
    // Инициализация генератора случайных чисел
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    // Установка локали для корректного вывода кириллицы
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif
    
    try {
        Menu menu;
        menu.run();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Неизвестная ошибка!\n";
        return 1;
    }
    
    return 0;
}