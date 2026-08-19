#pragma once
#include <iostream>
#include <string>
#include <algorithm>

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
    <script src="/test.js"></script>
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

std::string get_htm_chat(){
    std::string ret = "";

    std::string text = R"HTML(<!doctype html>
<html lang="ja">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Chat · 二十四節気</title>
    <!-- Tailwind via CDN -->
    <script src="https://cdn.tailwindcss.com"></script>
    <!-- HTMX via CDN -->
    <script src="https://unpkg.com/htmx.org@1.9.12"></script>
    <!-- Font Awesome (icons) -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css">
    <style>
        /* スクロールバーのカスタム（オプション） */
        .chat-scroll::-webkit-scrollbar {
            width: 6px;
        }
        .chat-scroll::-webkit-scrollbar-track {
            background: #1e293b;
        }
        .chat-scroll::-webkit-scrollbar-thumb {
            background: #475569;
            border-radius: 8px;
        }
        /* メッセージのアニメーション（さりげなく） */
        .msg-enter {
            animation: fadeSlide 0.25s ease-out;
        }
        @keyframes fadeSlide {
            0% { opacity: 0; transform: translateY(8px); }
            100% { opacity: 1; transform: translateY(0); }
        }
        /* 画像内の「Open Recent Project」風のバッジ */
        .project-badge {
            background: #0f172a;
            border: 1px solid #334155;
            color: #94a3b8;
            font-size: 0.75rem;
            padding: 0.2rem 0.8rem;
            border-radius: 20px;
            letter-spacing: 0.3px;
        }
        .project-badge i {
            color: #38bdf8;
            margin-right: 6px;
        }
        /* 入力エリアのフォーカス */
        .chat-input:focus {
            outline: none;
            box-shadow: 0 0 0 2px #38bdf8, 0 0 0 4px rgba(56, 189, 248, 0.2);
        }
        /* 左ペインのアイコンエリア */
        .sidebar-icon {
            width: 40px;
            height: 40px;
            display: flex;
            align-items: center;
            justify-content: center;
            border-radius: 10px;
            color: #94a3b8;
            transition: 0.2s;
        }
        .sidebar-icon:hover {
            background: #1e293b;
            color: #f1f5f9;
        }
        .sidebar-icon.active {
            background: #1e293b;
            color: #38bdf8;
        }
        .terminal-header {
            background: #0f172a;
            border-bottom: 1px solid #1e293b;
        }
        .terminal-window {
            background: #0b1120;
            border: 1px solid #1e293b;
            box-shadow: 0 12px 40px rgba(0,0,0,0.7);
        }
        /* ユーザーメッセージ / AIメッセージのスタイル */
        .message-user {
            background: #1e293b;
            border: 1px solid #334155;
            color: #e2e8f0;
        }
        .message-ai {
            background: #111827;
            border: 1px solid #1f2937;
            color: #d1d5db;
        }
        .message-ai code {
            background: #1e293b;
            padding: 0.1rem 0.4rem;
            border-radius: 6px;
            font-size: 0.85rem;
            color: #facc15;
        }
        .timestamp {
            font-size: 0.65rem;
            color: #64748b;
            margin-left: 12px;
        }
    </style>
</head>
<body class="bg-[#0a0f1a] text-gray-200 font-sans antialiased flex items-center justify-center min-h-screen p-4">

    <!-- メインウィンドウ: ターミナル風 + 左ペイン (Open Recent Project をイメージ) -->
    <div class="terminal-window rounded-2xl w-full max-w-6xl h-[90vh] flex overflow-hidden">

        <!-- ========== 左サイドバー (アイコン + プロジェクトバッジ) ========== -->
        <div class="w-20 bg-[#0b1120] border-r border-[#1e293b] flex flex-col items-center py-5 gap-4 flex-shrink-0">
            <!-- ロゴ/アイコン -->
            <div class="text-2xl mt-2 text-sky-400">
                <i class="fas fa-terminal"></i>
            </div>
            <!-- ナビゲーションアイコン (画像の "Open Recent Project" を連想) -->
            <div class="sidebar-icon active mt-6">
                <i class="fas fa-folder-open text-lg"></i>
            </div>
            <div class="sidebar-icon">
                <i class="fas fa-code text-lg"></i>
            </div>
            <div class="sidebar-icon mt-auto">
                <i class="fas fa-cog text-lg"></i>
            </div>
            <!-- 「Open Recent Project」バッジ (画像にあった要素) -->
            <div class="project-badge mt-2 flex items-center gap-1 px-3 py-1.5">
                <i class="fas fa-clock text-xs"></i>
                <span>Recent</span>
            </div>
        </div>

        <!-- ========== メインチャットエリア ========== -->
        <div class="flex-1 flex flex-col min-w-0 bg-[#0b1120]">

            <!-- ヘッダー: タイトル + ステータス (画像の "nika — pwsh.exe" 風) -->
            <div class="terminal-header px-6 py-3 flex items-center justify-between flex-shrink-0">
                <div class="flex items-center gap-3">
                    <i class="fas fa-comment-dots text-sky-400 text-sm"></i>
                    <span class="text-sm font-mono tracking-wide text-gray-300">chat</span>
                    <span class="text-xs text-gray-500 font-mono bg-[#1e293b] px-2 py-0.5 rounded-md">v0.1</span>
                </div>
                <div class="flex items-center gap-3 text-xs text-gray-400">
                    <span class="hidden sm:inline bg-[#1e293b] px-2 py-0.5 rounded-md">HTMX + Tailwind</span>
                </div>
            </div>

            <!-- ===== メッセージ表示領域 ===== -->
            <div id="message-container" class="flex-1 overflow-y-auto p-5 chat-scroll space-y-4 bg-[#0b1120]">
                <!-- 既存メッセージ（画像の会話を再現） -->
                <div class="message-ai msg-enter rounded-2xl px-5 py-3 max-w-3xl shadow-sm border border-[#1e293b]">
                    <div class="flex items-start gap-3">
                        <div class="w-7 h-7 rounded-full bg-sky-900/50 flex items-center justify-center text-sky-300 text-sm flex-shrink-0 mt-0.5">
                            <i class="fas fa-robot"></i>
                        </div>
                        <div>
                            <div class="flex items-center gap-2 flex-wrap">
                                <span class="font-medium text-sky-300 text-sm">AI</span>
                                <span class="timestamp">10:32</span>
                            </div>
                            <div class="mt-1 text-sm leading-relaxed text-gray-300">
                                こんにちは。ご挨拶ありがとうございます。これは日本語での日常的な挨拶であり、時間帯を問わず使われる丁寧な表現です。
                            </div>
                        </div>
                    </div>
                </div>

                <!-- ここに新着メッセージが挿入される (HTMX) -->
                <div id="new-messages" class="space-y-4"></div>
            </div>

            <!-- ===== 入力エリア (HTMX で送信) ===== -->
            <div class="border-t border-[#1e293b] p-4 bg-[#0f172a] flex-shrink-0">
                <form hx-post="/api/chat" 
                      hx-target="#new-messages" 
                      hx-swap="beforeend" 
                      hx-on::after-request="this.reset(); this.querySelector('input').focus();"
                      hx-headers='{"X-Requested-With": "XMLHttpRequest"}'
                      class="flex items-center gap-3">
                    
                    <div class="relative flex-1">
                        <input type="text" 
                               name="message" 
                               placeholder="Type your text and press Enter..." 
                               class="chat-input w-full bg-[#1a2332] border border-[#2a3a4a] rounded-2xl px-5 py-3 text-sm text-gray-200 placeholder:text-gray-500 focus:border-sky-500 transition-all"
                               autofocus>
                        <div class="absolute right-3 top-1/2 -translate-y-1/2 text-gray-500 text-xs flex items-center gap-1">
                            <kbd class="px-1.5 py-0.5 bg-[#0f172a] rounded border border-[#1e293b] text-[10px]">Enter</kbd>
                        </div>
                    </div>
                    
                    <button type="submit" 
                            class="bg-sky-600 hover:bg-sky-500 text-white rounded-full w-11 h-11 flex items-center justify-center transition-all shadow-lg shadow-sky-900/30 hover:shadow-sky-700/40 flex-shrink-0">
                        <i class="fas fa-paper-plane text-sm"></i>
                    </button>
                </form>
                <!-- 補足: 画像下部の "Type your text and press Enter:" 表示 -->
                <div class="text-[10px] text-gray-600 mt-2 px-1 tracking-wide flex items-center gap-2">
                    <i class="fas fa-chevron-right text-[8px]"></i>
                    <span>Type your text and press Enter:</span>
                    <span class="text-gray-500">(HTMX でリアルタイム送信)</span>
                </div>
            </div>
        </div>
    </div>

    <!-- ダミー送信をインターセプトするための設定： hx-post を打ち消すために、実際にはイベントで制御 -->
    <!-- 注意: 上記スクリプトで htmx:configRequest をインターセプトしているため、実際のサーバーリクエストは発生しません -->
    <!-- 本番環境ではサーバーサイドのエンドポイント /api/chat を実装してください -->
    <script src="/chat.js"></script>

</body>
</html>         
)HTML";
    //std::cout << text << std::endl;
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
