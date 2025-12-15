#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "node.h"
#include "container.h"
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <vector>

template <typename T>
class LinkedList : public Container<T> {
private:
    Node<T>* head;
    int size;
    
    // Вспомогательные методы
    Node<T>* getNode(int index) const {
        if (index < 0 || index >= size) {
            return nullptr;
        }
        
        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current;
    }
    
    // Меняет местами два узла по индексам
    void swapNodes(int i, int j) {
        if (i < 0 || i >= size || j < 0 || j >= size || i == j) {
            return;
        }
        
        // Получаем значения
        T temp = get(i);
        set(i, get(j));
        set(j, temp);
    }
    
    // Быстрая сортировка
    void quickSort(int left, int right, std::function<bool(const T&, const T&)> comparator) {
        if (left >= right) return;
        
        T pivot = get((left + right) / 2);
        int i = left;
        int j = right;
        
        while (i <= j) {
            while (comparator(get(i), pivot)) i++;
            while (comparator(pivot, get(j))) j--;
            
            if (i <= j) {
                swapNodes(i, j);
                i++;
                j--;
            }
        }
        
        if (left < j) quickSort(left, j, comparator);
        if (i < right) quickSort(i, right, comparator);
    }
    
public:
    // Конструкторы
    LinkedList() : head(nullptr), size(0) {}
    
    LinkedList(const LinkedList<T>& other) : head(nullptr), size(0) {
        Node<T>* current = other.head;
        while (current) {
            add(current->data);
            current = current->next;
        }
    }
    
    // Деструктор
    ~LinkedList() {
        clear();
    }
    
    // Оператор присваивания
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
    
    // Реализация виртуальных методов из Container
    virtual void add(const T& item) override {
        Node<T>* newNode = new Node<T>(item);
        
        if (!head) {
            head = newNode;
        } else {
            Node<T>* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }
    
    virtual void insert(int index, const T& item) override {
        if (index < 0 || index > size) {
            throw std::out_of_range("Index out of range");
        }
        
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
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        
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
        if (index != -1) {
            remove(index);
        }
    }
    
    virtual T get(int index) const override {
        Node<T>* node = getNode(index);
        if (!node) {
            throw std::out_of_range("Index out of range");
        }
        return node->data;
    }
    
    virtual void set(int index, const T& item) override {
        Node<T>* node = getNode(index);
        if (!node) {
            throw std::out_of_range("Index out of range");
        }
        node->data = item;
    }
    
    virtual int getSize() const override {
        return size;
    }
    
    virtual bool isEmpty() const override {
        return size == 0;
    }
    
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
            if (current->data == item) {
                return index;
            }
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
        if (!file.is_open()) {
            return false;
        }
        
        // Сохраняем количество элементов
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));
        
        // Сохраняем каждый элемент
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
        if (!file.is_open()) {
            return false;
        }
        
        clear();
        
        // Читаем количество элементов
        int fileSize;
        file.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
        
        // Читаем каждый элемент
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
            std::cout << "Элемент " << counter << ":\n";
            std::cout << current->data << "\n";
            current = current->next;
            counter++;
        }
    }
    
    // Дополнительные методы для работы со списком
    
    // Добавление с сохранением порядка (для отсортированного списка)
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
        
        // Если дошли до конца, добавляем в конец
        add(item);
    }
    
    // Поиск по предикату
    std::vector<T> findByPredicate(std::function<bool(const T&)> predicate) const {
        std::vector<T> results;
        Node<T>* current = head;
        
        while (current) {
            if (predicate(current->data)) {
                results.push_back(current->data);
            }
            current = current->next;
        }
        
        return results;
    }
    
    // Получение первого элемента
    T getFirst() const {
        if (!head) {
            throw std::runtime_error("List is empty");
        }
        return head->data;
    }
    
    // Получение последнего элемента
    T getLast() const {
        if (!head) {
            throw std::runtime_error("List is empty");
        }
        
        Node<T>* current = head;
        while (current->next) {
            current = current->next;
        }
        return current->data;
    }
    
    // Удаление первого элемента
    void removeFirst() {
        if (!head) return;
        remove(0);
    }
    
    // Удаление последнего элемента
    void removeLast() {
        if (!head) return;
        remove(size - 1);
    }
    
    // Преобразование в вектор
    std::vector<T> toVector() const {
        std::vector<T> result;
        Node<T>* current = head;
        
        while (current) {
            result.push_back(current->data);
            current = current->next;
        }
        
        return result;
    }
    
    // Оператор доступа по индексу
    T& operator[](int index) {
        Node<T>* node = getNode(index);
        if (!node) {
            throw std::out_of_range("Index out of range");
        }
        return node->data;
    }
    
    const T& operator[](int index) const {
        Node<T>* node = getNode(index);
        if (!node) {
            throw std::out_of_range("Index out of range");
        }
        return node->data;
    }
    
    // Итераторы (упрощенные)
    class Iterator {
    private:
        Node<T>* current;
        
    public:
        Iterator(Node<T>* node) : current(node) {}
        
        T& operator*() const { return current->data; }
        T* operator->() const { return &current->data; }
        
        Iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }
        
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
        
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };
    
    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};

#endif // LINKEDLIST_H