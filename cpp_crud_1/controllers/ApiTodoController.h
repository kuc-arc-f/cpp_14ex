#pragma once

#include <drogon/HttpController.h>
#include "./include/my_todo.hpp"
#include "./include/my_ssr.hpp"
#include "./include/todo_wrap.hpp"

using namespace drogon;

class ApiTodoController : public drogon::HttpController<ApiTodoController>
{
  public:
    METHOD_LIST_BEGIN
    // POST /todos        -> 新規登録
    ADD_METHOD_TO(ApiTodoController::create, "/api/todo/create", Post);
    // GET  /todos        -> 一覧取得
    ADD_METHOD_TO(ApiTodoController::list, "/api/todo/list", Get);
    //ADD_METHOD_TO(ApiTodoController::remove, "/todos/{1}", Delete);
    ADD_METHOD_TO(ApiTodoController::todo_remove, "/api/todo/delete", Post);
    METHOD_LIST_END

    void create(const HttpRequestPtr &req,
              std::function<void(const HttpResponsePtr &)> &&callback);

    void list(const HttpRequestPtr &req,
              std::function<void(const HttpResponsePtr &)> &&callback);
    void todo_remove(const HttpRequestPtr &req,
            std::function<void(const HttpResponsePtr &)> &&callback);          
    /*
    void remove(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback,
                int id);
    */

};
