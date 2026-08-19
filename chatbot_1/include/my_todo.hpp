#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"

#include "my_type.hpp"
#include "my_ssr.hpp"

using json = nlohmann::json;

const std::string FILE_PATH = "todos.json";

struct TodoData {
    int max_id;
    std::vector<Todo> items;
};

class MyTodo {
private:
    std::string m_name;

    public:
    explicit MyTodo(std::string str){}

    ~MyTodo() {}

    // JSONファイルからデータ読み込み
    TodoData load_data() {
        TodoData data{0, {}};
        std::ifstream file(FILE_PATH);
        if (file.is_open()) {
            try {
                json j = json::parse(file);
                if (j.contains("max_id") && j["max_id"].is_number()) {
                    data.max_id = j["max_id"].get<int>();
                }
                if (j.contains("items") && j["items"].is_array()) {
                    for (const auto& item : j["items"]) {
                        if (item.contains("id") && item.contains("title")) {
                            data.items.push_back({item["id"].get<int>(), item["title"].get<std::string>()});
                        }
                    }
                }
            } catch (const json::parse_error& e) {
                std::cerr << "JSON解析エラー: " << e.what() << "\n";
            }
        }
        return data;
    }

    // データをJSONファイルに保存
    void save_data(const TodoData& data) {
        json j;
        j["max_id"] = data.max_id;
        j["items"] = json::array();
        for (const auto& item : data.items) {
            j["items"].push_back({{"id", item.id}, {"title", item.title}});
        }
        std::ofstream file(FILE_PATH);
        if (file.is_open()) {
            file << j.dump(2); // インデント2で整形出力
            file.close();
        } else {
            std::cerr << "エラー: ファイルに書き込めません。\n";
        }
    }

    // TODO追加
    void add_todo(TodoData& data, const std::string& title) {
        data.max_id++;
        data.items.push_back({data.max_id, title});
        save_data(data);
        //std::cout << "add: #" << data.max_id << " " << title << "\n";
    }

    // TODO一覧表示
    void list_todos(const TodoData& data) {
        if (data.items.empty()) {
            std::cout << "TODO none\n";
            return;
        }
        for (const auto& item : data.items) {
            std::cout << "[" << item.id << "] " << item.title << "\n";
        }
    }   

    std::string dialog_show(int id) {
        std::string ret = "";

        TodoData data = load_data();
        if (data.items.empty()) {
            std::cout << "TODO none\n";
            return ret;
        }
        Todo row;
        row.id = 0;
        row.title = "";
        for (const auto& item : data.items) {
            if (item.id == id) {
                row.id = item.id;
                row.title = item.title;
            }
            std::cout << "[" << item.id << "] " << item.title << "\n";
        }

        MySsr sLib("");
        std::string out = sLib.renderDialog(row);
        ret = out;
        return ret;
    }   

    std::string list_json(const TodoData& data) {
        std::string ret = "";
        std::vector<Todo> todoItems;
        if (data.items.empty()) {
            std::cout << "TODO none\n";
            return "[]";
        }
        std::string out_str = "";
        for (const auto& item : data.items) {
            Todo row;
            row.id = item.id;
            row.title = item.title;
            todoItems.push_back(row);
            //std::string id_str = std::to_string(item.id);
            //out_str += "[" + id_str + "] "+ item.title + "\n";
            //std::cout << "[" << row.id << "] " << row.title << "\n";
        }
        json j1 = todoItems;
        std::string json_str = j1.dump();
        ret = json_str;
        return ret;
    }   

    // TODO削除
    void delete_todo(TodoData& data, int id) {
        auto it = std::remove_if(data.items.begin(), data.items.end(),
                                [id](const Todo& t) { return t.id == id; });
        if (it == data.items.end()) {
            //std::cout << "ID #" << id << " は存在しません。\n";
            std::cout << "ID #" << id << " none \n";
        } else {
            data.items.erase(it, data.items.end());
            save_data(data);
            //std::cout << "delete: #" << id << "\n";
        }
    }    
    
    std::string todo_add_handler(std::string input_str){
        std::string ret = "";
        try{
            TodoData data = load_data(); 
            add_todo(data, input_str);
            data = load_data(); 
            ret = list_json(data);            
            return ret;
        } catch (const std::exception& e) {
            std::cout << "Error , main" << std::endl;
            return ret;
        }  
    }

    std::string todo_list_handler(){
        std::string ret = "";
        try{
            TodoData data = load_data(); 
            ret = list_json(data);
            return ret;
        } catch (const std::exception& e) {
            std::cout << "Error , main" << std::endl;
            return ret;
        }  
    }

    std::string todo_list_elem(){
        std::string ret = "";
        try{
            TodoData data = load_data(); 
            //ret = list_json(data);
            MySsr sLib("");
            std::string resp = sLib.renderTodoList(data.items);
            ret = resp;
            return ret;
        } catch (const std::exception& e) {
            std::cout << "Error , main" << std::endl;
            return ret;
        }  
    }

    void todo_delete_handler(int id){
        try{
            TodoData data = load_data();
            delete_todo(data, id); 
        } catch (const std::exception& e) {
            std::cout << "Error , main" << std::endl;
        }  
    }

};
