#pragma once
#include <iostream>
#include "dotenv.h"
#include "openrouter_client.hpp"

class MyChat {
private:
    std::string m_name;
public:
    explicit MyChat(std::string str){}
    ~MyChat() {}

    std::string chat_send(std::string query){
        dotenv::init();
        std::string ret = "";
        try{
            // API_KEYを環境変数から取得（または直接設定）
            const char* api_key = std::getenv("OPENROUTER_API_KEY");
            if (api_key != nullptr) {
                //std::cout << "api_key:" << api_key << std::endl;
            }else{
                std::cerr << "Error: OPENROUTER_API_KEY environment variable not set" << std::endl;
                std::cerr << "Please set it with: export OPENROUTER_API_KEY=your_api_key_here" << std::endl;
                return ret;
            }      
            const char* model_name = std::getenv("OPENROUTER_MODEL");
            if (!model_name) {
                std::cerr << "Error: OPENROUTER_MODEL environment variable not set" << std::endl;
                return ret;
            }
            OpenRouterClient client(api_key);

            // チャットリクエストの送信
            auto response = client.sendChatCompletion(
                model_name,
                query,
                1.0,
                2000
            );

            if (response.has_value()) {
                //std::cout << "Response: " << response.value() << std::endl;
                auto outStr = response.value();
                ret = outStr;
                return ret;
            } else {
                std::cerr << "Failed to get response from API" << std::endl;
                return ret;
            }

            return ret;
        } catch (const std::exception& e) {
            std::cout << "Error , main" << std::endl;
        }  

        return ret;
    }

};
