
#include <iostream>  // Подключаем заголовочный файл для ввода-вывода
#include "sqlite3.h" // Подключаем заголовочный файл для работы с SQLite
#include <string>    // Подключаем заголовочный файл для работы со строками

int main()
{
    /* создадим указатели на объекты */
    sqlite3 *db;                                 // Указатель на базу данных SQLite
    char *errorMessage = nullptr;                // Указатель для сообщений об ошибках, инициализируем значением nullptr
    int exitCode = sqlite3_open("test.db", &db); // Открываем базу данных с именем "test.db" и сохраняем результат в exitCode

    /* выполним проверку создания или открытия базы данных*/
    if (exitCode)
    {                                                               // Проверяем успешность открытия базы данных
        std::cerr << "Can't open database: " << sqlite3_errmsg(db); // Выводим сообщение об ошибке
        return exitCode;                                            // Возвращаем exitCode и завершаем программу
    }
    else
    {
        std::cout << "Database opened successfully!\n"; // Выводим сообщение об успешном открытии базы данных
    }


    /* создадим текст для sql запроса который потом будем использовать, текс для создания таблицы */
    std::string createTableSQL = "CREATE TABLE IF NOT EXISTS MyTable(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);"; // Создаем SQL запрос для создания таблицы

    /* теперь выполним сам запрос */
    exitCode = sqlite3_exec(db, createTableSQL.c_str(), nullptr, 0, &errorMessage); // Выполняем SQL запрос для создания таблицы и сохраняем результат в exitCode

    /* выполним проверку на успешность создания таблицы или ее открытия */
    if (exitCode != SQLITE_OK)
    {                                               // Проверяем успешность создания таблицы
        std::cerr << "SQL error: " << errorMessage; // Выводим сообщение об ошибке
        sqlite3_free(errorMessage);                 // Освобождаем память, занятую errorMessage
    }
    else
    {
        std::cout << "Table created successfully\n"; // Выводим сообщение об успешном создании таблицы
    }


    /* теперь получим от пользователя данные, какие будет необходимо занести в таблицу  */
    int id;                    // Переменная для хранения введенного ID
    std::string name;          // Переменная для хранения введенного имени
    std::cout << "Enter ID: "; // Просим пользователя ввести ID
    std::cin >> id;            // Считываем введенный пользователем ID
    std::cin.ignore();         // Очищаем символ новой строки из буфера

    std::cout << "Enter name: ";  // Просим пользователя ввести имя
    std::getline(std::cin, name); // Считываем введенное пользователем имя

    /* создаем текс запроса на добавление данных в таблицу */
    std::string insertSQL = "INSERT INTO MyTable (id, name) VALUES (" + std::to_string(id) + ", '" + name + "');"; // Создаем SQL запрос для вставки данных
    /* так же можно добавить данные только в одну колонку */
    // std::string insertSQL = "INSERT INTO MyTable (id) VALUES (" + std::to_string(id) + " );"; // Создаем SQL запрос для вставки данных


    /* выполнение самого запроса добавления данных */
    exitCode = sqlite3_exec(db, insertSQL.c_str(), nullptr, 0, &errorMessage); // Выполняем SQL запрос для вставки данных и сохраняем результат в exitCode

    /* выполним проверку */
    if (exitCode != SQLITE_OK)
    {                                               // Проверяем успешность вставки данных
        std::cerr << "SQL error: " << errorMessage; // Выводим сообщение об ошибке
        sqlite3_free(errorMessage);                 // Освобождаем память, занятую errorMessage
    }
    else
    {
        std::cout << "Record inserted successfully\n"; // Выводим сообщение об успешной вставке данных
    }

    sqlite3_close(db); // Закрываем базу данных

    return 0; // Возвращаем 0, чтобы показать успешное завершение программы
}
