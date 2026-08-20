#pragma once
#include <drogon/drogon.h>
#include <iostream>
#include "./include/my_type.hpp"
#include "./include/my_db.hpp"
//#include "./include/my_todo.hpp"
#include "./include/my_ssr.hpp"

using namespace drogon;

const std::string DB_PATH = "todo.db";

class TodoWrap {
private:
    std::string m_name = "";

public:
    explicit TodoWrap(std::string str){}
    ~TodoWrap() {}

    std::string todo_create(const HttpRequestPtr &request){
        std::string ret = "";
        try{        
            Json::Value result;
            result["message"] = "user created";
            auto json = request->getJsonObject();
            std::string out_str = "";
            if (json) {
                Todo t;
                t.title       = (*json)["title"].asString();
                t.content     = (*json)["content"].asString();
                t.is_public   = (*json)["is_public"].asInt();
                t.food_orange = (*json)["food_orange"].asInt();
                t.food_apple  = (*json)["food_apple"].asInt();
                t.food_banana = (*json)["food_banana"].asInt();
                t.pub_date    = (*json)["pub_date"].asString();
                t.qty1        = (*json)["qty1"].asInt();
                t.qty2        = (*json)["qty2"].asInt();
                t.qty3        = (*json)["qty3"].asInt();

                if (t.title.empty()){
                    std::cout << "Error: title is required" << std::endl;
                    return ret;
                }
                MyDb dLib(DB_PATH);
                dLib.add(t);                
                result["ret"] = "OK";
            }          
          return out_str;
        } catch (const std::exception& e) {
            std::cout << "Error , todo_create" << std::endl;
            return ret;
        }
    }

    std::string todo_list(){
        try{
            //std::cout << "#todo_list_handler.start" << std::endl;
            MyDb db_helper(DB_PATH);
            auto todos = db_helper.list();
            //db_helper.print_table(todos);
            return db_helper.get_list_json(todos);
        } catch (const std::exception& e) {
            std::cout << "Error , list: " << e.what() << std::endl;
            return "[]";
        }  
    }

    int todo_delete_handler(const HttpRequestPtr &request)
    {
        int ret = -1;
        try{
            auto json = request->getJsonObject();
            int id = (*json)["id"].asInt();

            MyDb db_helper(DB_PATH);
            db_helper.remove(id);
            return 1;
        } catch (const std::exception& e) {
            std::cout << "Error , main" << std::endl;
            return ret;
        }  
    }    
  
};
