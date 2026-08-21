#include "ApiChatController.h"

using namespace drogon::orm;

void ApiChatController::send(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto json = req->getJsonObject();
    Json::Value result;
    result["message"] = "user created";
    std::string res2 = "";
    try
    {
        if (json) {
            MyChat cLib("");
            std::string query = (*json)["query"].asString();
            res2 = cLib.chat_send(query);
        }    
        auto resp = HttpResponse::newHttpResponse();
        resp->setBody(res2);
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