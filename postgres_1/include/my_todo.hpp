#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"

#include "my_db.hpp"

using json = nlohmann::json;

class MyTodo {
private:
    std::string m_name;

    public:
    explicit MyTodo(std::string str){}

    ~MyTodo() {}
    
    void todo_add_handler(std::string input_str){
        try{
            MyDb dLib("");
            PGconn* conn = dLib.connect_db();
            dLib.init_db(conn);
            dLib.cmd_add(conn, input_str);

            PQfinish(conn);
        } catch (const std::exception& e) {
            std::cout << "Error , main" << std::endl;
        }  
    }

    std::string todo_list_handler(){
        std::string ret = "";
        try{
            MyDb dLib("");
            PGconn* conn = dLib.connect_db();
            dLib.init_db(conn);
            ret = dLib.list_json(conn);

            return ret;
        } catch (const std::exception& e) {
            std::cout << "Error , main" << std::endl;
            return ret;
        }  
    }

    void delete_handler(int id){
        try{
            MyDb dLib("");
            PGconn* conn = dLib.connect_db();
            std::string id_str = std::to_string(id);
            dLib.cmd_delete(conn, id_str);
        } catch (const std::exception& e) {
            std::cout << "Error , main" << std::endl;
        }  
    }

};
