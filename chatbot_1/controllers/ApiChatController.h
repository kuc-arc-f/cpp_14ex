#pragma once

#include <drogon/HttpController.h>
#include "../include/my_chat.hpp"

using namespace drogon;

class ApiChatController : public drogon::HttpController<ApiChatController>
{
  public:
    METHOD_LIST_BEGIN
    // POST /todos        -> 新規登録
    //ADD_METHOD_TO(TodoController::create, "/todos", Post);
    ADD_METHOD_TO(ApiChatController::send, "/api/chat/send", Post);
    METHOD_LIST_END

    void send(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);
/*
    void list(const HttpRequestPtr &req,
              std::function<void(const HttpResponsePtr &)> &&callback);

    void remove(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback,
                int id);
*/
};
