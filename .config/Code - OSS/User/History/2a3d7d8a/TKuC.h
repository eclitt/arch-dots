#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include "linkedlist.h"
#include "student.h"
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <chrono>
#include <random>

class StudentManager {
private:
    LinkedList<Student> students;
    
public:
    // Конструкторы
    StudentManager() {}
    
    // Основные операции
    void addStudent(const Student& student) {
        students.add(student);
    }
    
    void insertStudent(int index, const Student& student) {
        students.insert(index, student);
    }
    
    void removeStudent(int index) {
        students.remove(index);
    }
    
    void removeStudentByLastName(const std::string& lastName) {
        for (int i = 0; i < students.getSize(); i++) {
            if (students.get(i).getLastName() == lastName) {
                students.remove(i);
                return;
            }
        }
    }
    
    Student getStudent(int index) const {
        return students.get(index);
    }
    
    void updateStudent(int index, const Student& student) {
        students.set(index, student);
    }
    
    int getStudentCount() const {
        return students.getSize();
    }
    
    bool isEmpty() const {
        return students.isEmpty();
    }
    
    void clearAll() {
        students.clear();
    }
    
    // Поиск
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
    
    // Сортировка
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
    
    // Добавление с сохранением порядка (по фамилии)
    void addStudentSorted(const Student& student, bool ascending = true) {
        students.addSorted(student, ascending);
    }
    
    // Статистика
    void printStatistics() const {
        if (students.isEmpty()) {
            std::cout << "Нет данных для статистики.\n";
            return;
        }
        
        int total = students.getSize();
        int currentlyStudying = searchCurrentlyStudying().size();
        
        // Статистика по группам
        std::vector<std::string> groups;
        for (int i = 0; i < students.getSize(); i++) {
            std::string group = students.get(i).getGroup();
            if (std::find(groups.begin(), groups.end(), group) == groups.end()) {
                groups.push_back(group);
            }
        }
        
        // Самый старший и самый младший
        Student oldest = students.get(0);
        Student youngest = students.get(0);
        for (int i = 1; i < students.getSize(); i++) {
            Student current = students.get(i);
            if (current.getBirthDate() < oldest.getBirthDate()) {
                oldest = current;
            }
            if (current.getBirthDate() > youngest.getBirthDate()) {
                youngest = current;
            }
        }
        
        std::cout << "=== Статистика ===\n";
        std::cout << "Всего студентов: " << total << "\n";
        std::cout << "Обучается в настоящее время: " << currentlyStudying << "\n";
        std::cout << "Количество групп: " << groups.size() << "\n";
        std::cout << "Самый старший студент: " << oldest.getLastName() 
                  << " (" << oldest.getBirthDate().getAge() << " лет)\n";
        std::cout << "Самый младший студент: " << youngest.getLastName() 
                  << " (" << youngest.getBirthDate().getAge() << " лет)\n";
        std::cout << "==================\n";
    }
    
    // Групповые операции
    void editByGroupAndPattern(const std::string& group, 
                               const std::string& pattern,
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
    
    // Сохранение/загрузка
    bool saveToFile(const std::string& filename) const {
        return students.saveToFile(filename);
    }
    
    bool loadFromFile(const std::string& filename) {
        return students.loadFromFile(filename);
    }
    
    // Вывод всех студентов
    void printAllStudents() const {
        std::cout << "=== Список студентов ===\n";
        students.print();
        std::cout << "========================\n";
    }
    
    // Вывод студентов по группе
    void printStudentsByGroup(const std::string& group) const {
        auto groupStudents = searchByGroup(group);
        
        if (groupStudents.empty()) {
            std::cout << "Студентов в группе " << group << " не найдено.\n";
            return;
        }
        
        std::cout << "=== Студенты группы " << group << " ===\n";
        for (const auto& student : groupStudents) {
            std::cout << student << "\n";
        }
        std::cout << "Всего: " << groupStudents.size() << " студентов\n";
    }
    
    // Генерация тестовых данных
    void generateTestData(int count) {
        std::vector<std::string> lastNames = {
            "Иванов", "Петров", "Сидоров", "Кузнецов", "Смирнов",
            "Попов", "Васильев", "Павлов", "Семенов", "Голубев",
            "Виноградов", "Богданов", "Воробьев", "Федоров", "Михайлов"
        };
        
        std::vector<std::string> groups = {
            "ИВТ-101", "ИВТ-102", "ИВТ-103",
            "ПМИ-201", "ПМИ-202", "ПМИ-203",
            "ФИИТ-301", "ФИИТ-302"
        };
        
        std::vector<std::string> addresses = {
            "ул. Ленина, 10", "ул. Советская, 25", "ул. Мира, 15",
            "ул. Пушкина, 7", "ул. Гагарина, 33", "ул. Кирова, 42",
            "ул. Комсомольская, 18", "ул. Школьная, 5"
        };
        
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
            if (i % 3 == 0) { // Добавляем индекс для разнообразия
                lastName += "-" + std::to_string(i % 10);
            }
            
            int birthYear = yearDist(gen);
            Date birthDate(dayDist(gen), monthDist(gen), birthYear);
            
            int admissionYear = birthYear + 18;
            Date admissionDate(1, 9, admissionYear);
            
            int expulsionYear = admissionYear + 4;
            Date expulsionDate(30, 6, expulsionYear);
            
            std::string address = addresses[addressDist(gen)];
            std::string group = groups[groupDist(gen)];
            
            Student student(lastName, birthDate, admissionDate, 
                           expulsionDate, address, group);
            
            students.add(student);
        }
    }
    
    // Тестирование производительности
    void testPerformance() {
        std::cout << "=== Тестирование производительности ===\n";
        
        if (students.isEmpty()) {
            std::cout << "Список пуст. Генерирую тестовые данные...\n";
            generateTestData(1000);
        }
        
        int n = students.getSize();
        std::cout << "Количество элементов: " << n << "\n";
        
        // Тест сортировки
        auto start = std::chrono::high_resolution_clock::now();
        sortByLastName();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Сортировка по фамилии: " << duration.count() << " мс\n";
        
        // Тест поиска
        start = std::chrono::high_resolution_clock::now();
        auto results = searchByGroup("ИВТ-101");
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Поиск по группе: " << duration.count() << " мс (найдено: " 
                  << results.size() << ")\n";
        
        // Тест вставки
        Student testStudent("Тестовый", Date(1, 1, 2000), Date(1, 9, 2020),
                           Date(30, 6, 2024), "ул. Тестовая, 1", "ТЕСТ-001");
        
        start = std::chrono::high_resolution_clock::now();
        students.add(testStudent);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Добавление в конец: " << duration.count() << " мс\n";
        
        // Тест удаления
        start = std::chrono::high_resolution_clock::now();
        if (n > 0) {
            students.remove(n / 2); // Удаляем из середины
        }
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Удаление из середины: " << duration.count() << " мс\n";
        
        std::cout << "=======================================\n";
    }
    
    // Получение доступа к списку
    LinkedList<Student>& getStudents() { return students; }
    const LinkedList<Student>& getStudents() const { return students; }
};

#endif // STUDENTMANAGER_H