#ifndef MENU_H
#define MENU_H

#include "studentmanager.h"
#include <iostream>
#include <limits>
#include <string>
#include <vector>

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
            std::cout << "Ошибка ввода. Пожалуйста, введите целое число от " 
                      << min << " до " << max << ".\n";
            clearInput();
        }
    }
    
    std::string getStringInput(const std::string& prompt) {
        std::string value;
        std::cout << prompt;
        std::getline(std::cin, value);
        return value;
    }
    
    Date getDateInput(const std::string& prompt) {
        std::cout << prompt << " (ДД ММ ГГГГ): ";
        int d, m, y;
        while (true) {
            if (std::cin >> d >> m >> y) {
                Date date(d, m, y);
                clearInput();
                return date;
            }
            std::cout << "Ошибка ввода. Пожалуйста, введите дату в формате ДД ММ ГГГГ: ";
            clearInput();
        }
    }
    
    Student inputStudent() {
        std::cout << "=== Ввод данных студента ===\n";
        
        std::string lastName = getStringInput("Фамилия: ");
        Date birthDate = getDateInput("Дата рождения");
        Date admissionDate = getDateInput("Дата поступления");
        Date expulsionDate = getDateInput("Дата отчисления");
        std::string address = getStringInput("Адрес: ");
        std::string group = getStringInput("Группа: ");
        
        return Student(lastName, birthDate, admissionDate, expulsionDate, address, group);
    }
    
    void printStudentList() {
        if (manager.isEmpty()) {
            std::cout << "Список студентов пуст.\n";
            return;
        }
        
        std::cout << "=== Список студентов ===\n";
        for (int i = 0; i < manager.getStudentCount(); i++) {
            std::cout << "[" << i + 1 << "] " << manager.getStudent(i).getLastName() 
                      << " (" << manager.getStudent(i).getGroup() << ")\n";
        }
    }
    
    void addStudentMenu() {
        std::cout << "=== Добавление студента ===\n";
        Student student = inputStudent();
        manager.addStudent(student);
        std::cout << "Студент успешно добавлен!\n";
    }
    
    void insertStudentMenu() {
        if (manager.isEmpty()) {
            std::cout << "Список пуст. Сначала добавьте студентов.\n";
            return;
        }
        
        printStudentList();
        int index = getIntInput("Введите позицию для вставки (1-" + 
                               std::to_string(manager.getStudentCount() + 1) + "): ", 
                               1, manager.getStudentCount() + 1);
        
        std::cout << "=== Ввод данных студента ===\n";
        Student student = inputStudent();
        manager.insertStudent(index - 1, student);
        std::cout << "Студент успешно вставлен!\n";
    }
    
    void removeStudentMenu() {
        if (manager.isEmpty()) {
            std::cout << "Список студентов пуст.\n";
            return;
        }
        
        printStudentList();
        int choice = getIntInput(
            "1. Удалить по номеру\n"
            "2. Удалить по фамилии\n"
            "3. Удалить по группе\n"
            "Выберите вариант: ", 1, 3);
        
        switch (choice) {
            case 1: {
                int index = getIntInput("Введите номер для удаления: ", 
                                       1, manager.getStudentCount());
                manager.removeStudent(index - 1);
                std::cout << "Студент успешно удален!\n";
                break;
            }
            case 2: {
                std::string lastName = getStringInput("Введите фамилию: ");
                manager.removeStudentByLastName(lastName);
                std::cout << "Студент(ы) успешно удален(ы)!\n";
                break;
            }
            case 3: {
                std::string group = getStringInput("Введите группу: ");
                manager.removeByGroup(group);
                std::cout << "Студенты группы " << group << " успешно удалены!\n";
                break;
            }
        }
    }
    
    void editStudentMenu() {
        if (manager.isEmpty()) {
            std::cout << "Список студентов пуст.\n";
            return;
        }
        
        printStudentList();
        int index = getIntInput("Введите номер студента для редактирования: ", 
                               1, manager.getStudentCount());
        
        std::cout << "Текущие данные студента:\n";
        std::cout << manager.getStudent(index - 1) << "\n";
        
        std::cout << "Введите новые данные:\n";
        Student newStudent = inputStudent();
        manager.updateStudent(index - 1, newStudent);
        std::cout << "Данные студента успешно обновлены!\n";
    }
    
    void searchMenu() {
        int choice = getIntInput(
            "=== Поиск студентов ===\n"
            "1. По группе\n"
            "2. По фамилии (точное совпадение)\n"
            "3. По шаблону фамилии\n"
            "4. Обучающиеся в настоящее время\n"
            "Выберите вариант: ", 1, 4);
        
        std::vector<Student> results;
        
        switch (choice) {
            case 1: {
                std::string group = getStringInput("Введите группу: ");
                results = manager.searchByGroup(group);
                break;
            }
            case 2: {
                std::string lastName = getStringInput("Введите фамилию: ");
                results = manager.searchByLastName(lastName);
                break;
            }
            case 3: {
                std::string pattern = getStringInput("Введите шаблон фамилии: ");
                results = manager.searchByLastNamePattern(pattern);
                break;
            }
            case 4: {
                results = manager.searchCurrentlyStudying();
                break;
            }
        }
        
        if (results.empty()) {
            std::cout << "Студенты не найдены.\n";
            return;
        }
        
        std::cout << "=== Результаты поиска (" << results.size() << " найден(о)) ===\n";
        for (const auto& student : results) {
            std::cout << student << "\n";
        }
    }
    
    void sortMenu() {
        int choice = getIntInput(
            "=== Сортировка студентов ===\n"
            "1. По фамилии (возрастание)\n"
            "2. По фамилии (убывание)\n"
            "3. По дате рождения\n"
            "4. По дате поступления\n"
            "5. По группе\n"
            "Выберите вариант: ", 1, 5);
        
        bool ascending = true;
        
        switch (choice) {
            case 1:
                manager.sortByLastName(true);
                break;
            case 2:
                manager.sortByLastName(false);
                ascending = false;
                break;
            case 3:
                ascending = getIntInput("1. Возрастание\n2. Убывание\nВыберите: ", 1, 2) == 1;
                manager.sortByBirthDate(ascending);
                break;
            case 4:
                ascending = getIntInput("1. Возрастание\n2. Убывание\nВыберите: ", 1, 2) == 1;
                manager.sortByAdmissionDate(ascending);
                break;
            case 5:
                ascending = getIntInput("1. Возрастание\n2. Убывание\nВыберите: ", 1, 2) == 1;
                manager.sortByGroup(ascending);
                break;
        }
        
        std::cout << "Сортировка завершена " 
                  << (ascending ? "(по возрастанию)" : "(по убыванию)") << "!\n";
    }
    
    void fileMenu() {
        int choice = getIntInput(
            "=== Работа с файлами ===\n"
            "1. Сохранить в файл\n"
            "2. Загрузить из файла\n"
            "Выберите вариант: ", 1, 2);
        
        std::string filename = getStringInput("Введите имя файла: ");
        
        if (choice == 1) {
            if (manager.saveToFile(filename)) {
                std::cout << "Данные успешно сохранены в файл " << filename << "\n";
            } else {
                std::cout << "Ошибка сохранения данных!\n";
            }
        } else {
            if (manager.loadFromFile(filename)) {
                std::cout << "Данные успешно загружены из файла " << filename << "\n";
            } else {
                std::cout << "Ошибка загрузки данных!\n";
            }
        }
    }
    
    void groupEditMenu() {
        std::string group = getStringInput("Введите группу для редактирования: ");
        std::string pattern = getStringInput("Введите шаблон фамилии: ");
        
        auto students = manager.searchByGroup(group);
        auto filtered = manager.searchByLastNamePattern(pattern);
        
        // Пересечение
        std::vector<Student> toEdit;
        for (const auto& s1 : students) {
            for (const auto& s2 : filtered) {
                if (s1.getLastName() == s2.getLastName()) {
                    toEdit.push_back(s1);
                    break;
                }
            }
        }
        
        if (toEdit.empty()) {
            std::cout << "Студенты не найдены по заданным критериям.\n";
            return;
        }
        
        std::cout << "Найдено " << toEdit.size() << " студент(ов) для редактирования:\n";
        for (const auto& student : toEdit) {
            std::cout << student.getLastName() << " (" << student.getGroup() << ")\n";
        }
        
        std::cout << "\nВведите новые данные для этих студентов:\n";
        Student newData = inputStudent();
        
        manager.editByGroupAndPattern(group, pattern, newData);
        std::cout << "Редактирование завершено!\n";
    }
    
    void addSortedMenu() {
        std::cout << "=== Добавление с сохранением порядка ===\n";
        Student student = inputStudent();
        
        int order = getIntInput("Порядок сортировки:\n1. По возрастанию\n2. По убыванию\nВыберите: ", 1, 2);
        bool ascending = (order == 1);
        
        manager.addStudentSorted(student, ascending);
        std::cout << "Студент добавлен с сохранением порядка сортировки!\n";
    }
    
public:
    void run() {
        std::cout << "=== Система управления студентами факультета ===\n";
        std::cout << "Используется односвязный список с ООП\n\n";
        
        while (true) {
            std::cout << "\n=== Главное меню ===\n";
            std::cout << "Текущее количество студентов: " << manager.getStudentCount() << "\n";
            std::cout << "1. Добавить студента\n";
            std::cout << "2. Вставить студента в позицию\n";
            std::cout << "3. Удалить студента\n";
            std::cout << "4. Редактировать студента\n";
            std::cout << "5. Показать всех студентов\n";
            std::cout << "6. Поиск студентов\n";
            std::cout << "7. Сортировка студентов\n";
            std::cout << "8. Статистика\n";
            std::cout << "9. Работа с файлами\n";
            std::cout << "10. Добавить с сохранением порядка\n";
            std::cout << "11. Редактировать по группе и шаблону фамилии\n";
            std::cout << "12. Тестирование производительности\n";
            std::cout << "13. Генерировать тестовые данные\n";
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
                    int count = getIntInput("Введите количество тестовых записей: ", 1, 10000);
                    manager.generateTestData(count);
                    std::cout << "Сгенерировано " << count << " тестовых записей.\n";
                    break;
                }
                default:
                    std::cout << "Неверный выбор!\n";
            }
        }
    }
};

#endif // MENU_H