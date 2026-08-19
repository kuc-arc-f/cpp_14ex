#pragma once
#include <iostream>
#include <string>
#include <algorithm>

//#include "my_type.hpp"

std::string ssr_htm_top(){
        std::string ret = "";
    std::string text = R"(<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>HTMX Todo App</title>
    <script src="https://unpkg.com/htmx.org@1.9.12"></script>
    <script src="https://cdn.jsdelivr.net/npm/@tailwindcss/browser@4"></script>
  </head>
  <body class="bg-stone-100 min-h-screen flex justify-center text-stone-900 font-sans">
    TOP
    <script src="/page_common.js"></script>
  </body>  
</html>
)";
    ret = text;
    return ret;
}

std::string html_replace(std::string html, std::string target, std::string change_str){
  size_t pos = html.find(target);
  if (pos != std::string::npos) {
      html.replace(pos, target.length(), change_str);
  }

  return html;
}


std::string ssr_htm_login(){
    std::string ret = "";

    std::string text = R"HTML(<!doctype html>
<html lang="ja">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ログイン画面</title>
  <script src="https://unpkg.com/htmx.org@1.9.12"></script>
  <script src="https://cdn.tailwindcss.com"></script>
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css">
</head>
<body class="bg-gray-100 flex items-center justify-center min-h-screen font-sans antialiased">
  <div class="w-full max-w-md p-8 bg-white rounded-2xl shadow-xl border border-gray-200/60 transition-all">
    
    <h1 class="text-3xl font-bold text-gray-800 text-center tracking-tight">Login</h1>
    <p class="text-gray-500 text-center mt-1 text-sm font-medium">アカウントにログインしてください</p>

    <form class="mt-8 space-y-5" 
        hx-post="/api/user/login"
        hx-trigger="submit"
        hx-target="#result_2"
        hx-on=""
        hx-on--after-request="after_login();">
      <div>
        <label for="email" class="block text-sm font-semibold text-gray-700 mb-1">メールアドレス</label>
        <div class="relative">
          <span class="absolute inset-y-0 left-3 flex items-center text-gray-400 text-sm">
            <i class="fas fa-envelope"></i>
          </span>
          <input type="email" id="email" name="email" value="" required
                 class="w-full pl-10 pr-4 py-3 border border-gray-300 rounded-xl text-gray-700 placeholder-gray-400 focus:outline-none focus:ring-2 focus:ring-blue-400 focus:border-transparent transition">
        </div>
      </div>

      <div>
        <div class="flex items-center justify-between mb-1">
          <label for="password" class="block text-sm font-semibold text-gray-700">パスワード</label>
          <a href="#" class="text-sm text-blue-600 hover:text-blue-800 font-medium hover:underline transition">お忘れですか？</a>
        </div>
        <div class="relative">
          <span class="absolute inset-y-0 left-3 flex items-center text-gray-400 text-sm">
            <i class="fas fa-lock"></i>
          </span>
          <input type="password" id="password" name="password" placeholder="パスワードを入力" required
            class="w-full pl-10 pr-4 py-3 border border-gray-300 rounded-xl text-gray-700 placeholder-gray-400 focus:outline-none focus:ring-2 focus:ring-blue-400 focus:border-transparent transition">
        </div>
      </div>

      <button type="submit" 
        class="w-full flex items-center justify-center gap-2 bg-blue-600 hover:bg-blue-700 text-white font-bold py-3.5 px-4 rounded-xl transition shadow-md hover:shadow-lg focus:outline-none focus:ring-2 focus:ring-blue-500 focus:ring-offset-2">
        <i class="fas fa-lock text-white text-sm"></i>
        ログイン
      </button>
      <div>
        <h3 id="result_2"></h3>
      </div>

      <div class="text-center text-sm text-gray-600 pt-2">
        アカウントをお持ちでないですか？ 
        <a href="#" class="text-blue-600 hover:text-blue-800 font-semibold hover:underline transition">新規登録</a>
      </div>
    </form>

    <div class="mt-4 text-center text-xs text-gray-400 border-t border-gray-100 pt-4">
      <span>セキュアなログイン 🔒</span>
    </div>
    <div id="user-container"></div>
  </div>
  <script>function after_login(){
    const v = document.getElementById("result_login").value
    if(v){ location.href = "/"; }
  }
  </script>
</body>
</html>
)HTML";
    //std::cout << text << std::endl;
    ret = text;
    return ret;
}