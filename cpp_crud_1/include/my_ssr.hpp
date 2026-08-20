#pragma once
#include <iostream>
#include <string>
#include <algorithm>

class MySsr {
private:
  std::string m_name = "";

public:
    explicit MySsr(std::string str){}
    ~MySsr() {}

    std::string html_replace(std::string html, std::string target, std::string change_str){
      size_t pos = html.find(target);
      if (pos != std::string::npos) {
          html.replace(pos, target.length(), change_str);
      }

      return html;
    }

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
    <script src="/js/client.js"></script>
  </head>
  <body class="bg-stone-100 min-h-screen flex justify-center text-stone-900 font-sans">
    <div class="max-w-3xl mx-auto p-4 py-12" hx-get="/api/todo/list" hx-trigger="load" hx-swap="outerHTML">
      Top
      <div class="flex justify-center mt-12">
        <div class="animate-spin rounded-full h-8 w-8 border-b-2 border-stone-800"></div>
      </div>
    </div>
  </body>  
</html>
)";
        ret = text;
        return ret;
    }

  std::string get_htm_todo(){
        std::string ret = "";

        std::string text = R"HTML(<!doctype html>
<html lang="ja">
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>TODOアプリ</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <!-- Font Awesome 6 (アイコン用) -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css" />
    <style>
        .d-none{ display: none; }
        /* 行にホバーしたときのエフェクト */
        .todo-row:hover {
            background-color: #f9fafb;
            cursor: pointer;
        }
        /* ラベルバッジのスタイル */
        .badge {
            font-size: 0.70rem;
            padding: 0.20rem 0.60rem;
            border-radius: 9999px;
            background-color: #e5e7eb;
            color: #1f2937;
        }
        .badge-urgent {
            background-color: #fecaca;
            color: #991b1b;
        }
        .badge-working {
            background-color: #fde68a;
            color: #92400e;
        }
        .badge-pending {
            background-color: #d1d5db;
            color: #374151;
        }
        /* ダイアログのオーバーレイ */
        .dialog-overlay {
            background-color: rgba(0, 0, 0, 0.4);
            backdrop-filter: blur(3px);
        }
        /* ダイアログアニメーション */
        .dialog-content {
            animation: fadeSlideIn 0.2s ease-out;
        }
        @keyframes fadeSlideIn {
            0% {
                opacity: 0;
                transform: scale(0.95) translateY(-10px);
            }
            100% {
                opacity: 1;
                transform: scale(1) translateY(0);
            }
        }
    </style>
</head>
<body class="bg-gray-100 flex items-center justify-center min-h-screen p-4 font-sans antialiased">

    <div class="w-full max-w-2xl bg-white rounded-2xl shadow-xl p-6 md:p-8 transition-all">
        <!-- ヘッダー -->
        <div class="flex items-center justify-between gap-3 mb-6">
            <h1 class="text-2xl font-bold text-gray-800 flex items-center gap-2">
                <i class="fas fa-list-check text-blue-500"></i> TODOリスト
            </h1>
            <button id="addTodoBtn" class="bg-blue-600 hover:bg-blue-700 text-white font-medium px-6 py-2.5 rounded-xl flex items-center justify-center gap-2 transition shadow-sm">
                <i class="fas fa-plus"></i> 追加
            </button>
        </div>

        <!-- リスト表示エリア -->
        <div id="todoListContainer" class="space-y-2 mt-2">
            <!-- ここに動的にTODOアイテムが表示されます -->
            <p class="text-gray-400 text-sm text-center py-8" id="emptyMessage">
                <i class="fas fa-inbox mr-2"></i> TODOがありません。追加してみましょう！
            </p>
        </div>

        <!-- フッター情報 -->
        <div class="mt-5 text-xs text-gray-400 border-t border-gray-100 pt-4 flex justify-between items-center">
            <span><i class="far fa-circle mr-1"></i> 行をクリックで詳細を表示 / 編集 で内容を変更できます</span>
            <span id="todoCount" class="bg-gray-200 px-3 py-0.5 rounded-full text-gray-600 text-xs">0件</span>
        </div>
    </div>

    <!-- 追加ダイアログ -->
    <div id="addDialog" class="fixed inset-0 flex items-center justify-center z-50 dialog-overlay hidden transition-opacity">
        <div class="bg-white w-full max-w-lg max-h-[90vh] overflow-y-auto mx-4 rounded-2xl shadow-2xl dialog-content p-6 relative">
            <button id="closeAddDialogBtn" class="absolute top-3 right-4 text-gray-400 hover:text-gray-700 transition text-xl">
                <i class="fas fa-times"></i>
            </button>

            <h2 class="text-xl font-semibold text-gray-800 mb-4 flex items-center gap-2">
                <i class="fas fa-plus text-blue-500"></i> TODOを追加
            </h2>

            <div class="space-y-4">
                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">タイトル</label>
                    <input type="text" id="addTitle" placeholder="やることを入力..." class="w-full px-4 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400" />
                </div>

                <div class="d-none">
                    <label class="block text-sm font-medium text-gray-700 mb-1">詳細メモ</label>
                    <textarea id="addMemo" rows="3" class="w-full px-4 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400 resize-none" placeholder="詳細な説明を入力..."></textarea>
                </div>

                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">内容</label>
                    <input type="text" id="content" placeholder="内容を入力..." class="w-full px-4 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400" />
                </div>

                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">公開設定</label>
                    <div class="flex items-center gap-6">
                        <label class="flex items-center gap-2 text-sm text-gray-700 cursor-pointer">
                            <input type="radio" name="public" value="公開" checked class="accent-blue-600" /> 公開
                        </label>
                        <label class="flex items-center gap-2 text-sm text-gray-700 cursor-pointer">
                            <input type="radio" name="public" value="非公開" class="accent-blue-600" /> 非公開
                        </label>
                    </div>
                </div>

                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">食材</label>
                    <div class="flex flex-wrap gap-4">
                        <label class="flex items-center gap-2 text-sm text-gray-700 cursor-pointer">
                            <input type="checkbox" id="food_orange" class="accent-orange-500" /> オレンジ
                        </label>
                        <label class="flex items-center gap-2 text-sm text-gray-700 cursor-pointer">
                            <input type="checkbox" id="food_apple" class="accent-red-500" /> りんご
                        </label>
                        <label class="flex items-center gap-2 text-sm text-gray-700 cursor-pointer">
                            <input type="checkbox" id="food_banana" class="accent-yellow-500" /> バナナ
                        </label>
                    </div>
                </div>

                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">公開日</label>
                    <input type="date" id="pub_date" class="w-full px-4 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400 bg-white" />
                </div>

                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">数量</label>
                    <div class="grid grid-cols-3 gap-3">
                        <input type="number" id="qty1" placeholder="数量1" class="w-full px-3 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400" />
                        <input type="number" id="qty2" placeholder="数量2" class="w-full px-3 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400" />
                        <input type="number" id="qty3" placeholder="数量3" class="w-full px-3 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400" />
                    </div>
                </div>

                <div class="flex flex-col sm:flex-row gap-3 pt-2">
                    <button id="registerTodoBtn" class="flex-1 bg-blue-600 hover:bg-blue-700 text-white font-medium py-2.5 rounded-xl transition shadow-sm flex items-center justify-center gap-2">
                        <i class="fas fa-check"></i> 登録
                    </button>
                    <button id="cancelAddBtn" class="flex-1 bg-gray-200 hover:bg-gray-300 text-gray-700 font-medium py-2.5 rounded-xl transition shadow-sm flex items-center justify-center gap-2">
                        <i class="fas fa-xmark"></i> キャンセル
                    </button>
                </div>
            </div>
        </div>
    </div>

    <!-- 詳細/編集ダイアログ -->
    <div id="detailDialog" class="fixed inset-0 flex items-center justify-center z-50 dialog-overlay hidden transition-opacity">
        <div class="bg-white w-full max-w-lg max-h-[90vh] overflow-y-auto mx-4 rounded-2xl shadow-2xl dialog-content p-6 relative">
            <!-- 閉じるボタン -->
            <button id="closeDialogBtn" class="absolute top-3 right-4 text-gray-400 hover:text-gray-700 transition text-xl">
                <i class="fas fa-times"></i>
            </button>

            <h2 class="text-xl font-semibold text-gray-800 mb-4 flex items-center gap-2">
                <i class="fas fa-pen-to-square text-blue-500"></i> TODO詳細 / 編集
            </h2>

            <!-- 編集フォーム -->
            <div class="space-y-4">
                <!-- タイトル -->
                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">タイトル</label>
                    <input type="text" id="editTitle" class="w-full px-4 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400" />
                </div>

                <!-- ステータス (ラベル) 選択 -->
                <div class="d-none">
                    <label class="block text-sm font-medium text-gray-700 mb-1">ステータス</label>
                    <select id="editStatus" class="w-full px-4 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400 bg-white">
                        <option value="未着手"># 未着手</option>
                        <option value="作業中"># 作業中</option>
                        <option value="緊急"># 緊急</option>
                        <option value="完了"># 完了</option>
                    </select>
                </div>

                <!-- 詳細メモ -->
                <div class="d-none">
                    <label class="block text-sm font-medium text-gray-700 mb-1">詳細メモ</label>
                    <textarea id="editMemo" rows="3" class="w-full px-4 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400 resize-none" placeholder="詳細な説明を入力..."></textarea>
                </div>

                <!-- 内容 -->
                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">内容</label>
                    <input type="text" id="editContent" placeholder="内容を入力..." class="w-full px-4 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400" />
                </div>

                <!-- 公開設定 -->
                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">公開設定</label>
                    <div class="flex items-center gap-6">
                        <label class="flex items-center gap-2 text-sm text-gray-700 cursor-pointer">
                            <input type="radio" name="editPublic" value="公開" checked class="accent-blue-600" /> 公開
                        </label>
                        <label class="flex items-center gap-2 text-sm text-gray-700 cursor-pointer">
                            <input type="radio" name="editPublic" value="非公開" class="accent-blue-600" /> 非公開
                        </label>
                    </div>
                </div>

                <!-- 食材 -->
                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">食材</label>
                    <div class="flex flex-wrap gap-4">
                        <label class="flex items-center gap-2 text-sm text-gray-700 cursor-pointer">
                            <input type="checkbox" id="editFoodOrange" class="accent-orange-500" /> オレンジ
                        </label>
                        <label class="flex items-center gap-2 text-sm text-gray-700 cursor-pointer">
                            <input type="checkbox" id="editFoodApple" class="accent-red-500" /> りんご
                        </label>
                        <label class="flex items-center gap-2 text-sm text-gray-700 cursor-pointer">
                            <input type="checkbox" id="editFoodBanana" class="accent-yellow-500" /> バナナ
                        </label>
                    </div>
                </div>

                <!-- 公開日 -->
                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">公開日</label>
                    <input type="date" id="editPubDate" class="w-full px-4 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400 bg-white" />
                </div>

                <!-- 数量 -->
                <div>
                    <label class="block text-sm font-medium text-gray-700 mb-1">数量</label>
                    <div class="grid grid-cols-3 gap-3">
                        <input type="number" id="editQty1" placeholder="数量1" class="w-full px-3 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400" />
                        <input type="number" id="editQty2" placeholder="数量2" class="w-full px-3 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400" />
                        <input type="number" id="editQty3" placeholder="数量3" class="w-full px-3 py-2 border border-gray-300 rounded-xl focus:outline-none focus:ring-2 focus:ring-blue-400" />
                    </div>
                </div>

                <!-- アクションボタン -->
                <div class="flex flex-col sm:flex-row gap-3 pt-2">

                    <button id="deleteTodoBtn" class="flex-1 bg-red-500 hover:bg-red-600 text-white font-medium py-2.5 rounded-xl transition shadow-sm flex items-center justify-center gap-2">
                        <i class="fas fa-trash-can"></i> 削除
                    </button>
                </div>
            </div>
        </div>
    </div>

    <script src="/client.js"></script>
</body>
</html>
)HTML";
        //std::cout << text << std::endl;
        ret = text;
        return ret;
    }    

    std::string get_htm_about(){
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
  <body class="bg-white min-h-screen text-stone-900 font-sans">
    <div>
      <a href="/" class="font-bold ms-4" >Home</a>
      <a href="/about" class="ms-4" >[ about ]</a>
      <hr class="my-2" />
    </div>      
    <div class="max-w-3xl mx-auto p-4 py-12" >
      <h1 class="font-bold text-xl" >About</h1>
      <hr /> 
    </div>
  </body>  
</html>
)";
        //std::cout << text << std::endl;
        ret = text;
        return ret;
    }


};