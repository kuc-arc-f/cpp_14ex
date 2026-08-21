#include "ApiTodoController.h"

using namespace drogon::orm;

void ApiTodoController::create(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto json = req->getJsonObject();
    if (!json || !json->isMember("title") || (*json)["title"].asString().empty())
    {
        Json::Value err;
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("title is required");
        callback(resp);
        return;
    }
    try
    {
        TodoWrap tw("");
        auto res1 = tw.todo_create(req);
        auto resp = HttpResponse::newHttpResponse();
        resp->setBody(res1);
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

void ApiTodoController::list(const HttpRequestPtr &req,
                           std::function<void(const HttpResponsePtr &)> &&callback)
{
    //auto dbClient = app().getDbClient();
    try
    {
        TodoWrap tLib("");
        auto items = tLib.todo_list();
        auto resp = HttpResponse::newHttpResponse();
        resp->setBody(items);
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

void ApiTodoController::todo_remove(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback)
{
    try
    {
        //auto id = req->getParameter("id");
        auto json = req->getJsonObject();
       std::string id = (*json)["id"].asString();
        if (id.empty())
        {
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("id is required");
            callback(resp);        
            return;
        }        
        TodoWrap tLib("");
        tLib.todo_delete_handler(req);

        auto resp = HttpResponse::newHttpResponse();
        resp->setBody("OK");
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
