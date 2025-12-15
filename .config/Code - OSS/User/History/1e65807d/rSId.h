#ifndef CONTAINER_H
#define CONTAINER_H

#include <functional>

template <typename T>
class Container {
public:
    virtual ~Container() {}
    
    // Основные операции
    virtual void add(const T& item) = 0;
    virtual void insert(int index, const T& item) = 0;
    virtual void remove(int index) = 0;
    virtual void remove(const T& item) = 0;
    virtual T get(int index) const = 0;
    virtual void set(int index, const T& item) = 0;
    
    // Информация
    virtual int getSize() const = 0;
    virtual bool isEmpty() const = 0;
    virtual void clear() = 0;
    
    // Поиск
    virtual int find(const T& item) const = 0;
    virtual bool contains(const T& item) const = 0;
    
    // Сортировка
    virtual void sort(bool ascending = true) = 0;
    virtual void sort(std::function<bool(const T&, const T&)> comparator) = 0;
    
    // Сохранение/загрузка
    virtual bool saveToFile(const std::string& filename) const = 0;
    virtual bool loadFromFile(const std::string& filename) = 0;
    
    // Вывод
    virtual void print() const = 0;
};

#endif // CONTAINER_H