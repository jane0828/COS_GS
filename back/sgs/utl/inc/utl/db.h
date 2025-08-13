#ifndef _UTL_DB_H_
#define _UTL_DB_H_
#include <iostream>
#include <mysql/mysql.h>

class MySQLDatabase {
private:
    std::string host, user, password, database;
    MYSQL *conn;

public:
    MySQLDatabase(const std::string& host, const std::string& user, const std::string& password, const std::string& database);
    bool connect();
    void readData(const std::string& table);
    void insertData(const std::string& table, const std::string& column, const std::string& value);
    void close();
    ~MySQLDatabase();
};
#endif