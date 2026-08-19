#include <drogon/drogon.h>
#include "include/my_ssr.hpp"
#include "include/my_user.hpp"

using namespace drogon;

int main() {
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 5555);

    app().registerHandler(
        "/", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            std::string htm = ssr_htm_top();
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody(htm);
            callback(resp);
        }, {Get});    
    app().registerHandler(
        "/test", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            auto resp = HttpResponse::newHttpResponse();

            Cookie cookie("cpp_login_1", "1");
            cookie.setPath("/");
            cookie.setMaxAge(3600 * 24 * 30);  // 1H * 24 * N day
            cookie.setHttpOnly(false);
            resp->addCookie(cookie);
            callback(resp);
        }, {Get}); 
    app().registerHandler(
        "/login", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            std::string htm = ssr_htm_login();
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody(htm);
            callback(resp);
        }, {Get});    
    app().registerHandler(
        "/api/user/login", [](const HttpRequestPtr &request,  std::function<void(const HttpResponsePtr &)> &&callback) {
            std::string htm = ssr_htm_login();
            auto email = request->getParameter("email");
            auto password = request->getParameter("password");

            MyUser mLib("");
            int ret1 = mLib.user_login(email, password);
            auto resp = HttpResponse::newHttpResponse();
            std::string body_str = R"(<div class="font-bold text-2xl bg-red-400 text-white p-2">Error</div>)";
            if(ret1 > 0){
                Cookie cookie("cpp_login_1", "1");
                cookie.setPath("/");
                cookie.setMaxAge(3600 * 24 * 30);  // 1H * 24 * N day
                cookie.setHttpOnly(false);
                resp->addCookie(cookie);
                body_str = R"(<div><input type="text" id="result_login" value="1" /></div>)";
            }
            resp->setBody(body_str);
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
