#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// ─────────────────────────────────────────
//  Data model
// ─────────────────────────────────────────
struct Todo {
    int         id;
    std::string title;
    std::string content;
    int         is_public;
    int         food_orange;
    int         food_apple;
    int         food_banana;
    std::string pub_date;
    int         qty1;
    int         qty2;
    int         qty3;
    std::string created_at;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Todo, id, title, content, is_public, food_orange, food_apple, food_banana, pub_date, qty1, qty2, qty3, created_at)
