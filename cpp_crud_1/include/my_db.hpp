#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <sqlite3.h>
#include <nlohmann/json.hpp>
#include "my_type.hpp"

using json = nlohmann::json;

/*
struct Todo {
    int         id;
    std::string title;
    std::string content;
    int         is_public;
    int         food_orange;
    int         food_apple;
    int         food_banana;
    std::string pub_date;
    int         qty1;
    int         qty2;
    int         qty3;
    std::string created_at;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Todo, id, title, content, is_public, food_orange, food_apple, food_banana, pub_date, qty1, qty2, qty3, created_at)
*/
/*
inline void to_json(json& j, const Todo& t) {
    j = json{
        {"id", t.id},
        {"title", t.title},
        {"content", t.content},
        {"public", t.is_public},
        {"food_orange", t.food_orange},
        {"food_apple", t.food_apple},
        {"food_banana", t.food_banana},
        {"pub_date", t.pub_date},
        {"qty1", t.qty1},
        {"qty2", t.qty2},
        {"qty3", t.qty3},
        {"created_at", t.created_at}
    };
}
*/

class MyDb {
private:
    std::string m_name;
    sqlite3* db_ = nullptr;

    void exec(const std::string& sql) {
        char* err = nullptr;
        if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
            std::string msg = err ? err : "unknown";
            sqlite3_free(err);
            die(msg);
        }
    }

    void prepare(const std::string& sql, sqlite3_stmt** s) {
        if (sqlite3_prepare_v2(db_, sql.c_str(), -1, s, nullptr) != SQLITE_OK)
            die(sqlite3_errmsg(db_));
    }

    void step_and_finalize(sqlite3_stmt* s) {
        sqlite3_step(s);
        sqlite3_finalize(s);
    }

    static std::string timestamp() {
        std::time_t t = std::time(nullptr);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
        return buf;
    }

    [[noreturn]] static void die(const std::string& msg) {
        std::cerr << "DB error: " << msg << "\n";
        std::exit(1);
    }

public:
    explicit MyDb(const std::string& path){
        if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK)
            die("open");
        exec(
            "CREATE TABLE IF NOT EXISTS todos ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "title TEXT NOT NULL, "
            "content TEXT, "
            "public INTEGER NOT NULL DEFAULT 0, "
            "food_orange INTEGER DEFAULT 0, "
            "food_apple INTEGER DEFAULT 0, "
            "food_banana INTEGER DEFAULT 0, "
            "pub_date TEXT, "
            "qty1 INTEGER DEFAULT 0, "
            "qty2 INTEGER DEFAULT 0, "
            "qty3 INTEGER DEFAULT 0, "
            "created_at TEXT DEFAULT (DATETIME('now', 'localtime'))"
            ");"
        );
    }

    ~MyDb() { sqlite3_close(db_); }


    // ── Write ──────────────────────────────
    void add(const Todo& t) {
        sqlite3_stmt* s;
        prepare(
            "INSERT INTO todos "
            "(title, content, public, food_orange, food_apple, food_banana, pub_date, qty1, qty2, qty3) "
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);",
            &s);
        sqlite3_bind_text(s, 1, t.title.c_str(), -1, SQLITE_TRANSIENT);
        if (t.content.empty()) sqlite3_bind_null(s, 2);
        else sqlite3_bind_text(s, 2, t.content.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(s, 3, t.is_public);
        sqlite3_bind_int(s, 4, t.food_orange);
        sqlite3_bind_int(s, 5, t.food_apple);
        sqlite3_bind_int(s, 6, t.food_banana);
        if (t.pub_date.empty()) sqlite3_bind_null(s, 7);
        else sqlite3_bind_text(s, 7, t.pub_date.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(s, 8, t.qty1);
        sqlite3_bind_int(s, 9, t.qty2);
        sqlite3_bind_int(s, 10, t.qty3);
        step_and_finalize(s);
        std::cout << "✓ 追加しました: [" << sqlite3_last_insert_rowid(db_) << "] " << t.title << "\n";
    }

    void remove(int id) {
        sqlite3_stmt* s;
        prepare("DELETE FROM todos WHERE id = ?;", &s);
        sqlite3_bind_int(s, 1, id);
        step_and_finalize(s);
        if (sqlite3_changes(db_) == 0)
            std::cout << "ID " << id << " が見つかりません。\n";
        else
            std::cout << "✓ 削除しました: ID " << id << "\n";
    }

    // ── Read ───────────────────────────────
    std::vector<Todo> list() {
        std::string sql =
            "SELECT id, title, content, public, food_orange, food_apple, "
            "food_banana, pub_date, qty1, qty2, qty3, created_at FROM todos ORDER BY id;";
        std::cout << "sql=" << sql << std::endl;

        sqlite3_stmt* s;
        prepare(sql, &s);
        std::vector<Todo> rows;
        //char* val = nullptr;
        while (sqlite3_step(s) == SQLITE_ROW) {
            const unsigned char* content = sqlite3_column_text(s, 2);
            const unsigned char* pub_date = sqlite3_column_text(s, 7);
            Todo t;
            t.id          = sqlite3_column_int(s, 0);
            t.title       = reinterpret_cast<const char*>(sqlite3_column_text(s, 1));
            //if (t.content.empty()) { t.content = ""; }
            //else{ t.content = reinterpret_cast<const char*>(sqlite3_column_text(s, 2));};
            if (content != nullptr) {
                std::string val(reinterpret_cast<const char*>(content));
                t.content = val;
            }
            t.is_public   = sqlite3_column_int(s, 3);
            t.food_orange = sqlite3_column_int(s, 4);
            t.food_apple  = sqlite3_column_int(s, 5);
            t.food_banana = sqlite3_column_int(s, 6);
            //if (t.pub_date.empty()) { t.pub_date = ""; }
            //else{ t.pub_date = reinterpret_cast<const char*>(sqlite3_column_text(s, 7));};
            if (pub_date != nullptr) {
                std::string val(reinterpret_cast<const char*>(pub_date));
                t.pub_date = val;
            }
            t.qty1        = sqlite3_column_int(s, 8);
            t.qty2        = sqlite3_column_int(s, 9);
            t.qty3        = sqlite3_column_int(s, 10);
            t.created_at  = reinterpret_cast<const char*>(sqlite3_column_text(s, 11));
            rows.push_back(t);
        }
        sqlite3_finalize(s);
        return rows;
    }

    std::string get_list_json(const std::vector<Todo>& todos) {
        if (todos.empty()) {
            std::cout << "  (タスクはありません)\n";
            return "[]";
        }

        json j1 = todos;
        return j1.dump();
    }

    // ─────────────────────────────────────────
    //  Display
    // ─────────────────────────────────────────
    void print_table(const std::vector<Todo>& todos) {
        if (todos.empty()) {
            std::cout << "  (タスクはありません)\n";
            return;
        }
        // header
        /*
        std::cout << "\n"
                << "  " << std::left
                << std::setw(5)  << "ID"
                << std::setw(6)  << "公開"
                << std::setw(30) << "タイトル"
                << std::setw(8)  << "橙"
                << std::setw(8)  << "林檎"
                << std::setw(8)  << "芭蕉"
                << "作成日時" << "\n"
                << "  " << std::string(85, '-') << "\n";
        */

        for (const auto& t : todos) {
            std::string title = t.title;
            if (title.size() > 27) title = title.substr(0, 24) + "...";
            std::cout << "  "
                    << std::setw(5)  << t.id
                    //<< std::setw(6)  << (t.is_public ? "✔" : "○")
                    << ",title=" << title
                    << ",content=" << t.content
                    << std::setw(8)  << t.food_orange
                    << std::setw(8)  << t.food_apple
                    << ",food_banana="  << t.food_banana
                    << ",pub_date=" << t.pub_date
                    << ",qty1=" << t.qty1
                    << ",qty2=" << t.qty2
                    << ",qty3=" << t.qty3
                    << ",created_at=" << t.created_at << "\n";
        }
        std::cout << "\n";
    }

};
