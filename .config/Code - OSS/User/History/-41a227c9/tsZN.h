#ifndef STUDENT_H
#define STUDENT_H

#include "date.h"
#include <string>
#include <iostream>
#include <sstream>

class Student {
private:
    std::string lastName;
    Date birthDate;
    Date admissionDate;
    Date expulsionDate;
    std::string address;
    std::string group;
    
public:
    // Конструкторы
    Student() : lastName(""), birthDate(1, 1, 2000), 
                admissionDate(1, 9, 2020), expulsionDate(30, 6, 2024),
                address(""), group("") {}
    
    Student(const std::string& ln, const Date& bd, const Date& ad, 
            const Date& ed, const std::string& addr, const std::string& gr)
        : lastName(ln), birthDate(bd), admissionDate(ad), 
          expulsionDate(ed), address(addr), group(gr) {}
    
    // Геттеры
    std::string getLastName() const { return lastName; }
    Date getBirthDate() const { return birthDate; }
    Date getAdmissionDate() const { return admissionDate; }
    Date getExpulsionDate() const { return expulsionDate; }
    std::string getAddress() const { return address; }
    std::string getGroup() const { return group; }
    
    // Сеттеры
    void setLastName(const std::string& ln) { lastName = ln; }
    void setBirthDate(const Date& bd) { birthDate = bd; }
    void setAdmissionDate(const Date& ad) { admissionDate = ad; }
    void setExpulsionDate(const Date& ed) { expulsionDate = ed; }
    void setAddress(const std::string& addr) { address = addr; }
    void setGroup(const std::string& gr) { group = gr; }
    
    // Операторы сравнения (по фамилии)
    bool operator<(const Student& other) const {
        return lastName < other.lastName;
    }
    
    bool operator>(const Student& other) const {
        return lastName > other.lastName;
    }
    
    bool operator==(const Student& other) const {
        return lastName == other.lastName && 
               birthDate == other.birthDate &&
               group == other.group;
    }
    
    bool operator!=(const Student& other) const {
        return !(*this == other);
    }
    
    // Сравнение по дате рождения
    bool compareByBirthDate(const Student& other) const {
        return birthDate < other.birthDate;
    }
    
    // Сравнение по дате поступления
    bool compareByAdmissionDate(const Student& other) const {
        return admissionDate < other.admissionDate;
    }
    
    // Сравнение по группе (если группы равны - по фамилии)
    bool compareByGroup(const Student& other) const {
        if (group != other.group) {
            return group < other.group;
        }
        return lastName < other.lastName;
    }
    
    // Сериализация
    void serialize(std::ostream& os) const {
        // Сохраняем длину фамилии и саму фамилию
        size_t len = lastName.length();
        os.write(reinterpret_cast<const char*>(&len), sizeof(len));
        os.write(lastName.c_str(), len);
        
        // Сохраняем даты
        birthDate.serialize(os);
        admissionDate.serialize(os);
        expulsionDate.serialize(os);
        
        // Сохраняем адрес
        len = address.length();
        os.write(reinterpret_cast<const char*>(&len), sizeof(len));
        os.write(address.c_str(), len);
        
        // Сохраняем группу
        len = group.length();
        os.write(reinterpret_cast<const char*>(&len), sizeof(len));
        os.write(group.c_str(), len);
    }
    
    // Десериализация
    void deserialize(std::istream& is) {
        // Читаем фамилию
        size_t len;
        is.read(reinterpret_cast<char*>(&len), sizeof(len));
        char* buffer = new char[len + 1];
        is.read(buffer, len);
        buffer[len] = '\0';
        lastName = buffer;
        delete[] buffer;
        
        // Читаем даты
        birthDate.deserialize(is);
        admissionDate.deserialize(is);
        expulsionDate.deserialize(is);
        
        // Читаем адрес
        is.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        is.read(buffer, len);
        buffer[len] = '\0';
        address = buffer;
        delete[] buffer;
        
        // Читаем группу
        is.read(reinterpret_cast<char*>(&len), sizeof(len));
        buffer = new char[len + 1];
        is.read(buffer, len);
        buffer[len] = '\0';
        group = buffer;
        delete[] buffer;
    }
    
    // Вывод информации о студенте
    friend std::ostream& operator<<(std::ostream& os, const Student& student) {
        os << "Фамилия: " << student.lastName << "\n"
           << "Дата рождения: " << student.birthDate 
           << " (возраст: " << student.birthDate.getAge() << " лет)\n"
           << "Дата поступления: " << student.admissionDate << "\n"
           << "Дата отчисления: " << student.expulsionDate << "\n"
           << "Адрес: " << student.address << "\n"
           << "Группа: " << student.group << "\n"
           << "Курс: ";
        
        // Вычисляем курс
        Date current = Date::getCurrentDate();
        int course = current.getYear() - student.admissionDate.getYear();
        if (current.getMonth() < 9) {
            course--;
        }
        if (course < 1) course = 1;
        if (course > 5) course = 5;
        
        os << course << "\n";
        os << "--------------------------------------------------\n";
        return os;
    }
    
    // Ввод информации о студенте
    friend std::istream& operator>>(std::istream& is, Student& student) {
        std::cout << "Введите фамилию: ";
        std::getline(is, student.lastName);
        
        std::cout << "Введите дату рождения (ДД ММ ГГГГ): ";
        int d, m, y;
        is >> d >> m >> y;
        is.ignore();
        student.birthDate = Date(d, m, y);
        
        std::cout << "Введите дату поступления (ДД ММ ГГГГ): ";
        is >> d >> m >> y;
        is.ignore();
        student.admissionDate = Date(d, m, y);
        
        std::cout << "Введите дату отчисления (ДД ММ ГГГГ): ";
        is >> d >> m >> y;
        is.ignore();
        student.expulsionDate = Date(d, m, y);
        
        std::cout << "Введите адрес: ";
        std::getline(is, student.address);
        
        std::cout << "Введите группу: ";
        std::getline(is, student.group);
        
        return is;
    }
    
    // Проверка, учится ли студент сейчас
    bool isCurrentlyStudying() const {
        Date current = Date::getCurrentDate();
        return current >= admissionDate && current <= expulsionDate;
    }
    
    // Получение информации в виде строки
    std::string toString() const {
        std::ostringstream oss;
        oss << *this;
        return oss.str();
    }
};

#endif // STUDENT_H