#include <drogon/drogon.h>
#include "./include/my_todo.hpp"

using namespace drogon;

int main() {
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 5555);

    app().registerHandler(
        "/api/todo/create", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            auto json = request->getJsonObject();
            Json::Value result;
            result["message"] = "user created";
            MyTodo tLib("");
            if (json) {
                MyTodo tLib("");
                std::string title = (*json)["title"].asString();
                tLib.todo_add_handler(title);
                result["ret"] = "OK";
            }            
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);            
        }, {Post});    
    app().registerHandler(
        "/api/todo/delete", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            auto json = request->getJsonObject();
            Json::Value result;
            result["message"] = "user created";
            MyTodo tLib("");
            if (json) {
                std::string id = (*json)["id"].asString();
                tLib.delete_handler(std::stoi(id));
                result["ret"] = "OK";
            }
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);             
        }, {Post});   

    app().registerHandler(
        "/api/todo/list", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            MyTodo tLib("");
            auto items = tLib.todo_list_handler();
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody(items);
            callback(resp);
        }, {Get});

        //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
