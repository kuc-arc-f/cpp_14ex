#include <drogon/drogon.h>
#include "./include/my_todo.hpp"
#include "./include/my_ssr.hpp"
#include "./include/todo_wrap.hpp"

using namespace drogon;

int main() {
    // config.json を読み込む(リスンポート、DB接続情報など)
    drogon::app().loadConfigFile("config.json");    
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
        
    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().setDocumentRoot("./static");
    drogon::app().run();
    return 0;
}
