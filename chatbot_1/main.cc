#include <drogon/drogon.h>
#include "./include/my_chat.hpp"
#include "./include/my_ssr.hpp"

using namespace drogon;

int main() {
    // config.json を読み込む(リスンポート、DB接続情報など)
    drogon::app().loadConfigFile("config.json");    

    app().registerHandler(
        "/", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            std::string htm = get_htm_chat();
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody(htm);
            callback(resp);
        }, {Get});
    /*
    app().registerHandler(
        "/api/chat/send", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            auto json = request->getJsonObject();
            Json::Value result;
            result["message"] = "user created";
            std::string res2 = "";
            if (json) {
                MyChat cLib("");
                std::string query = (*json)["query"].asString();
                res2 = cLib.chat_send(query);
            }
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody(res2);
            callback(resp);
        }, {Post});   
    */

    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //drogon::app().loadConfigFile("../config.yaml");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().setDocumentRoot("./static");
    LOG_INFO << "Server starting on 0.0.0.0:8080";
    drogon::app().run();
    return 0;
}
