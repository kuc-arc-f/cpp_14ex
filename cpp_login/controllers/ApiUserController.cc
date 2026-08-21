#include "ApiUserController.h"
//#include "include/my_ssr.hpp"
#include "include/my_user.hpp"

using namespace drogon::orm;

void ApiUserController::user_login(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto email = req->getParameter("email");
    auto password = req->getParameter("password");
    try
    {
        auto resp = HttpResponse::newHttpResponse();
        MyUser mLib("");
        int ret1 = mLib.user_login(email, password);
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
    }
    catch (const DrogonDbException &e)
    {
        Json::Value err;
        err["error"] = e.base().what();
        auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    }
}

