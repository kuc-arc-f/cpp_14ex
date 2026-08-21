#pragma once

#include <drogon/HttpController.h>
#include "include/my_user.hpp"

using namespace drogon;

class ApiUserController : public drogon::HttpController<ApiUserController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ApiUserController::user_login, "/api/user/login", Post);
    METHOD_LIST_END

    void user_login(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

};
