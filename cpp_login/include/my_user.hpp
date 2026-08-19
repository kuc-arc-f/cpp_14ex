#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "nlohmann/json.hpp"
#include "dotenv.h"

using json = nlohmann::json;

struct UserReq {
    std::string email;
    std::string password;
};

class MyUser {
private:
    std::string m_name;

public:
    explicit MyUser(std::string str){}
    ~MyUser() {}

    int user_login(std::string email, std::string password){
        int ret = -1;
        dotenv::init(); 

        char* env_user_mail = std::getenv("USER_MAIL");
        if (env_user_mail) {
            std::cout << "env_user_mail: " << env_user_mail << std::endl;
        }
        char* env_user_pass = std::getenv("USER_PASS");
        if (env_user_pass) {
            std::cout << "env_user_pass: " << env_user_pass << std::endl;
        }
        //json j1 = json::parse(body);
        //std::string email = j1.at("email").get<std::string>();
        //std::string password = j1.at("password").get<std::string>();
        std::cout << "email: " << email << std::endl;
        if(env_user_mail != email){
            std::cout << "error, user_mail NG"  << std::endl;
            return ret;
        }
        if(env_user_pass != password){
            std::cout << "error, password NG"  << std::endl;
            return ret;
        }
        
        ret = 1;
        return ret;
    }

};

