#include <drogon/drogon.h>
#include "./include/my_todo.hpp"
#include "./include/my_ssr.hpp"
#include "./include/todo_wrap.hpp"

using namespace drogon;

int main() {
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 5555);

    app().registerHandler(
        "/", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            MySsr ss("");
            std::string htm = ss.get_htm_todo();
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody(htm);
            callback(resp);
        }, {Get});
    app().registerHandler(
        "/api/todo/list", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            TodoWrap tLib("");
            auto items = tLib.todo_list();
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody(items);
            callback(resp);
        }, {Get});
    app().registerHandler(
        "/api/todo/create", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            TodoWrap tw("");
            auto res1 = tw.todo_create(request);
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody(res1);
            callback(resp);            
        }, {Post});      
    app().registerHandler(
        "/api/todo/delete", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            TodoWrap tw("");
            tw.todo_delete_handler(request);
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("OK");
            callback(resp);            
        }, {Post});

    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().setDocumentRoot("./static");
    drogon::app().run();
    return 0;
}
