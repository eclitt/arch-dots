#include "linkedlist_manager.h"
#include <limits>
#include <climits>

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
                std::cout <<