#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <libpq-fe.h>
#include <iostream>
#include "nlohmann/json.hpp"
#include "dotenv.h"

using json = nlohmann::json;

struct Todo {
    int id;
    std::string title;
};
// これ一行で、QueryReq <=> json の変換が魔法のように可能になります
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Todo, id, title)

class MyDb {
private:
    std::string m_name;

public:
    explicit MyDb(std::string str){}
    ~MyDb() {}

    // ─────────────────────────────────────────
    //  DB connection
    // ─────────────────────────────────────────
    PGconn* connect_db() {
        dotenv::init();
        char* database_url = std::getenv("DATABASE_URL");
        if (database_url) {
            std::cout << "database_url: " << database_url << std::endl;
        }else{
            std::cout << "error , database_url none" << std::endl;
            return nullptr;
        }
        std::string conninfo = database_url;
        std::cout << "Connectiing=" << conninfo << "\n";

        PGconn* conn = PQconnectdb(conninfo.c_str());
        if (PQstatus(conn) != CONNECTION_OK) {
            std::cerr << "Connection failed: " << PQerrorMessage(conn) << "\n";
            PQfinish(conn);
            std::exit(1);
        }
        return conn;
    }

    // ─────────────────────────────────────────
    //  Init table
    // ─────────────────────────────────────────
    void init_db(PGconn* conn) {
        const char* sql =
            "CREATE TABLE IF NOT EXISTS todos ("
            "  id        SERIAL PRIMARY KEY,"
            "  title     TEXT        NOT NULL,"
            "  done      BOOLEAN     NOT NULL DEFAULT FALSE,"
            "  created_at TIMESTAMPTZ NOT NULL DEFAULT NOW()"
            ");";
        PGresult* res = PQexec(conn, sql);
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            std::cerr << "init_db failed: " << PQerrorMessage(conn) << "\n";
            PQclear(res);
            PQfinish(conn);
            std::exit(1);
        }
        PQclear(res);
    }

    // ─────────────────────────────────────────
    //  Commands
    // ─────────────────────────────────────────
    void cmd_add(PGconn* conn, const std::string& title) {
        const char* params[1] = { title.c_str() };
        PGresult* res = PQexecParams(conn,
            "INSERT INTO todos (title) VALUES ($1) RETURNING id",
            1, nullptr, params, nullptr, nullptr, 0);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "add failed: " << PQerrorMessage(conn) << "\n";
        } else {
            std::cout << "Added todo #" << PQgetvalue(res, 0, 0)
                    << ": " << title << "\n";
        }
        PQclear(res);
    }

    void cmd_list(PGconn* conn) {
        PGresult* res = PQexec(conn,
            "SELECT id, done, title, created_at FROM todos ORDER BY id");

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "list failed: " << PQerrorMessage(conn) << "\n";
            PQclear(res);
            return;
        }

        int rows = PQntuples(res);
        if (rows == 0) {
            std::cout << "No todos yet. Use: todo add \"your task\"\n";
            PQclear(res);
            return;
        }

        std::cout << "\n";
        std::cout << std::left
                << std::setw(5)  << "ID"
                << std::setw(6)  << "Done"
                << std::setw(40) << "Title"
                << "Created\n";
        std::cout << std::string(70, '-') << "\n";

        for (int i = 0; i < rows; i++) {
            std::string id      = PQgetvalue(res, i, 0);
            std::string done    = PQgetvalue(res, i, 1);
            std::string title   = PQgetvalue(res, i, 2);
            std::string created = PQgetvalue(res, i, 3);

            // trim timestamp to date+time
            if (created.size() > 19) created = created.substr(0, 19);

            std::string mark = (done == "t") ? "[x]" : "[ ]";
            std::cout << std::left
                    << std::setw(5)  << id
                    << std::setw(6)  << mark
                    << std::setw(40) << title
                    << created << "\n";
        }
        std::cout << "\n";
        PQclear(res);
    }


    std::string list_json(PGconn* conn) {
        std::string ret = "";
        PGresult* res = PQexec(conn,
            "SELECT id, done, title, created_at FROM todos ORDER BY id");

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "list failed: " << PQerrorMessage(conn) << "\n";
            PQclear(res);
            return ret;
        }

        int rows = PQntuples(res);
        if (rows == 0) {
            std::cout << "No todos yet. Use: todo add \"your task\"\n";
            PQclear(res);
            return ret;
        }

        //std::cout << "\n";
        std::vector<Todo> todo_items;
        for (int i = 0; i < rows; i++) {
            std::string id      = PQgetvalue(res, i, 0);
            std::string done    = PQgetvalue(res, i, 1);
            std::string title   = PQgetvalue(res, i, 2);
            std::string created = PQgetvalue(res, i, 3);

            // trim timestamp to date+time
            if (created.size() > 19) created = created.substr(0, 19);

            Todo row;
            row.id = std::stoi(id);
            row.title = title;
            todo_items.push_back(row);
        }
        PQclear(res);

        json j1 = todo_items; 
        ret = j1.dump();

        return ret;
    }    

    void cmd_done(PGconn* conn, const std::string& id) {
        const char* params[1] = { id.c_str() };
        PGresult* res = PQexecParams(conn,
            "UPDATE todos SET done = TRUE WHERE id = $1 RETURNING title",
            1, nullptr, params, nullptr, nullptr, 0);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "done failed: " << PQerrorMessage(conn) << "\n";
        } else if (PQntuples(res) == 0) {
            std::cout << "Todo #" << id << " not found.\n";
        } else {
            std::cout << "Marked done: #" << id
                    << " " << PQgetvalue(res, 0, 0) << "\n";
        }
        PQclear(res);
    }

    void cmd_delete(PGconn* conn, const std::string& id) {
        const char* params[1] = { id.c_str() };
        PGresult* res = PQexecParams(conn,
            "DELETE FROM todos WHERE id = $1 RETURNING title",
            1, nullptr, params, nullptr, nullptr, 0);

        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            std::cerr << "delete failed: " << PQerrorMessage(conn) << "\n";
        } else if (PQntuples(res) == 0) {
            std::cout << "Todo #" << id << " not found.\n";
        } else {
            std::cout << "Deleted: #" << id
                    << " " << PQgetvalue(res, 0, 0) << "\n";
        }
        PQclear(res);
    }    

};





/*
void usage(const char* prog) {
    std::cout
        << "Usage:\n"
        << "  " << prog << " add \"<title>\"   Add a new todo\n"
        << "  " << prog << " list             List all todos\n"
        << "  " << prog << " done <id>        Mark todo as done\n"
        << "  " << prog << " delete <id>      Delete a todo\n"
        << "\nEnvironment variables:\n"
        << "  PGHOST       Postgres host     (default: localhost)\n"
        << "  PGPORT       Postgres port     (default: 5432)\n"
        << "  PGDATABASE   Database name\n"
        << "  PGUSER       Username\n"
        << "  PGPASSWORD   Password\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }

    std::string cmd = argv[1];

    PGconn* conn = connect_db();
    init_db(conn);

    if (cmd == "add") {
        if (argc < 3) {
            std::cerr << "Error: 'add' requires a title.\n";
            std::cerr << "  Example: todo add \"Buy milk\"\n";
            PQfinish(conn);
            return 1;
        }
        std::string title;
        for (int i = 2; i < argc; i++) {
            if (i > 2) title += " ";
            title += argv[i];
        }
        cmd_add(conn, title);

    } else if (cmd == "list") {
        cmd_list(conn);

    } else if (cmd == "done") {
        if (argc < 3) {
            std::cerr << "Error: 'done' requires an id.\n";
            PQfinish(conn);
            return 1;
        }
        cmd_done(conn, argv[2]);

    } else if (cmd == "delete") {
        if (argc < 3) {
            std::cerr << "Error: 'delete' requires an id.\n";
            PQfinish(conn);
            return 1;
        }
        cmd_delete(conn, argv[2]);

    } else {
        std::cerr << "Unknown command: " << cmd << "\n\n";
        usage(argv[0]);
        PQfinish(conn);
        return 1;
    }

    PQfinish(conn);
    return 0;
}
*/
