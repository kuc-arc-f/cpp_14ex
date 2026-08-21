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

};
