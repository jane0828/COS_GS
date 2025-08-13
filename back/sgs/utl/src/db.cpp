
#include <utl/utl.h>

MySQLDatabase::MySQLDatabase(const std::string& host, const std::string& user, const std::string& password, const std::string& database)
    : host(host), user(user), password(password), database(database), conn(nullptr) {}

bool MySQLDatabase::connect() {
    conn = mysql_init(nullptr);
    if (!conn) {
        std::cerr << "MySQL initialization failed!" << std::endl;
        return false;
    }

    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), password.c_str(), database.c_str(), 3306, nullptr, 0)) {
        std::cerr << "Connection error: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        return false;
    }
    std::cout << "Connected to MySQL server successfully!" << std::endl;
    return true;
}

void MySQLDatabase::readData(const std::string& table) {
    std::string query = "SELECT * FROM " + table;
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Read error: " << mysql_error(conn) << std::endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) {
        std::cerr << "Error storing result: " << mysql_error(conn) << std::endl;
        return;
    }

    int num_fields = mysql_num_fields(res);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < num_fields; i++) {
            std::cout << (row[i] ? row[i] : "NULL") << " ";
        }
        std::cout << std::endl;
    }
    mysql_free_result(res);
}

void MySQLDatabase::insertData(const std::string& table, const std::string& column, const std::string& value) {
    std::string query = "INSERT INTO " + table + " (" + column + ") VALUES ('" + value + "')";
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Insert error: " << mysql_error(conn) << std::endl;
        return;
    }
    std::cout << "Data inserted successfully!" << std::endl;
}

void MySQLDatabase::close() {
    if (conn) {
        mysql_close(conn);
        conn = nullptr;
        std::cout << "Disconnected from MySQL." << std::endl;
    }
}

MySQLDatabase::~MySQLDatabase() {
    close();
}
