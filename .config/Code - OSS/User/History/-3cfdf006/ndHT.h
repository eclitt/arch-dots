#ifndef NODE_H
#define NODE_H

template <typename T>
class Node {
public:
    T data;
    Node<T>* next;
    
    // Конструктор
    Node(const T& data) : data(data), next(nullptr) {}
    
    // Конструктор копирования
    Node(const Node<T>& other) : data(other.data), next(nullptr) {}
    
    // Деструктор
    ~Node() {
        // next удаляется в LinkedList
    }
};

#endif // NODE_H