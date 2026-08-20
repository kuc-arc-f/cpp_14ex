#pragma once
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "my_db.hpp"

using json = nlohmann::json;

class MyTodo {
private:
    std::string m_name;

    public:
    explicit MyTodo(std::string str){}

    ~MyTodo() {}
    /*
    void todo_add_handler(std::string input_str){
        try{
            std::cout << "todo_add_handler.input_str=" << input_str << std::endl;

            json j1 = json::parse(input_str);
            Todo t;
            t.title       = j1.value("title", "");
            t.content     = j1.value("content", "");
            t.is_public   = j1.value("public", 0);
            t.food_orange = j1.value("food_orange", 0);
            t.food_apple  = j1.value("food_apple", 0);
            t.food_banana = j1.value("food_banana", 0);
            t.pub_date    = j1.value("pub_date", "");
            t.qty1        = j1.value("qty1", 0);
            t.qty2        = j1.value("qty2", 0);
            t.qty3        = j1.value("qty3", 0);

            if (t.title.empty()){
                std::cout << "Error: title is required" << std::endl;
                return;
            }

            MyDb db_helper(DB_PATH);
            db_helper.add(t);
        } catch (const std::exception& e) {
            std::cout << "Error , add: " << e.what() << std::endl;
        }  
    }
    */

};
