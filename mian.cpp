#include <iostream>
#include "sql_src/sqlite3.h"

int main()
{
    /* создаем переменную и на объект базы и на результат открытия */
    sqlite3 *db;                                 // создаем указатель на объект базы данных
    int rc = sqlite3_open("mydatabase.db", &db); // открываем базу данных, если ее нет, то создаем ее, одновременно с этим заносим результат в переменную rc

    /* теперь выполняем проверку на соединине с базой данных */
    if (rc != SQLITE_OK)
    {
        std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db); // закрываем базу данных
    }
    else
    {
        std::cout << "Opened database successfully" << std::endl;
    }

    /* создаем текст для запроса на создание таблицы если ее не существует*/
    const char *sql_req = "CREATE TABLE IF NOT EXISTS start_table (id INTGER PRIMARY KEY, name TEXT);";

    /* создаем запрос на добавление данных в нашу таблицу */
    // const char *sql_insert = "INSERT INTO start_table (id, name) VALUES (2, 'Don Gondon');"; // SQL запрос для добавления записи в таблицу "start_table" с указанными значениями
    const char *sql_insert = "INSERT INTO start_table (id, name) VALUES (1, 'Don Pidoron');"; // SQL запрос для добавления записи в таблицу "start_table" с указанными значениями

    /* объявлем указатель на для сообщения об ошибках */
    char *err_msg = 0;

    /* выполняем запрос на создание таблицы */
    int rc_table = sqlite3_exec(db, sql_req, 0, 0, &err_msg);

    /* далее выполняем проверку на успешное создание таблицы */
    if (rc_table != SQLITE_OK) // выполняем проверку на успешное создание таблицы
    {
        std::cerr << "SQL error: " << err_msg << std::endl; // если все плохо, то выводим сообщение об ошибке
        sqlite3_free(err_msg);                              // освобождаем память
    }
    else
    {
        std::cout << "Table created successfully" << std::endl; // если все хорошо, то выводим сообщение об успешном создании таблицы
    }

    /* выполняем запрос на добавление данных в таблицу */
    int rc_insert = sqlite3_exec(db, sql_insert, 0, 0, &err_msg);

    /* далее выполняем проверку на успешное добавление данных в таблицу*/
    if (rc_insert != SQLITE_OK) // выполняем проверку на успешное создание таблицы
    {
        std::cerr << "SQL error: " << err_msg << std::endl; // если все плохо, то выводим сообщение об ошибке
        sqlite3_free(err_msg);                              // освобождаем память
    }
    else
    {
        std::cout << "Data added successfully" << std::endl; // если все хорошо, то выводим сообщение об успешном создании таблицы
    }

    /* ДОБАВЛЕНИЕ ДАННЫХ С ПОМОЩЮЬ ПОДГОТОВЛЕННОГО ЗАПРОСА */
    /* для начала создадим так называемый подготовленный запрос по мне так это шаблон */
    const char *sql_insert_template = "INSERT INTO start_table (id, name) VALUES (?,?);"; // SQL запрос для добавления записи в таблицу "start_table"

    /* далее нам необходимо объявить структуру для подготовленного запроса */
    sqlite3_stmt *stmt; // объявляем структуру для подготовленного запроса

    int rc_prepare = sqlite3_prepare_v2(db, sql_insert_template, -1, &stmt, nullptr); // подготовка подготовленого запроса

    /* выполним проверку подготовленного запроса */
    if (rc_prepare != SQLITE_OK)    //проверка результата подготовленного запроса
    {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;  //вывод сообщения об ошибке подготовленного запроса
    }

    /* создадим переменные которые мы отправим в sql запрос */
    int id = 2; //установка значения для параметра id
    const char *name = "Alice"; //установка значения для параметра name

    /* ВНИМАНИЕ */
    /* эта функция используется для привязки целочисленных значений к параметрам запроса */
    sqlite3_bind_int(stmt, 1, id);
    /* привязываем значение к парамметру name */
    sqlite3_bind_text(stmt, 2, name, -1, SQLITE_STATIC);

    int rc_done = sqlite3_step(stmt); // выполняем запрос на добавление данных

     if (rc_done != SQLITE_DONE) {  // Проверка результата выполнения запроса
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl; // Вывод сообщения об ошибке выполнения запроса
    } else {
        std::cout << "Record inserted successfully." << std::endl; // Вывод сообщения об успешном добавлении записи
    }

    sqlite3_finalize(stmt); // Освобождение ресурсов, связанных с подготовленным запросом
    sqlite3_close(db); // закрываем базу данных
    return 0;
}
