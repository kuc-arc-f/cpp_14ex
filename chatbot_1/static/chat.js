console.log("#chat_js");
async function sendPostRequest(data) {
  try {
    const response = await fetch('/api/chat/send', {
      method: 'POST', // リクエストメソッドを指定
      headers: {
        'Content-Type': 'application/json' // JSONを送ることを伝える
      },
      body: JSON.stringify(data) // データをJSON文字列に変換
    });

    // レスポンスのステータスコードを確認
    if (!response.ok) {
      throw new Error(`HTTPエラー! ステータス: ${response.status}`);
    }

    // 返ってきたJSONデータを解析
    const result = await response.text();
    //console.log('成功:', result);
    return result;
  } catch (error) {
    console.error('エラーが発生しました:', error);
  }
}
// HTMX リクエストをインターセプトしてダミーレスポンスを返す (本番はサーバー側で処理)
document.addEventListener('htmx:configRequest', function(evt) {
    //  /api/chat へのPOSTをインターセプト
    if (evt.detail.path === '/api/chat' && evt.detail.verb === 'post') {
        evt.preventDefault(); // 実際のリクエストを止める
        const form = evt.detail.elt;
        const input = form.querySelector('input[name="message"]');
        const userMessage = input.value.trim();
        console.log("userMessage=", userMessage);
        if (!userMessage) return;

        // ユーザーメッセージを即座に表示 (HTMXのtargetに追加)
        const container = document.getElementById('new-messages');
        const userDiv = document.createElement('div');
        userDiv.className = 'message-user msg-enter rounded-2xl px-5 py-3 max-w-3xl ml-auto shadow-sm border border-[#1e293b] bg-[#1a2332]';
        userDiv.innerHTML = `
            <div class="flex items-start gap-3 flex-row-reverse">
                <div class="w-7 h-7 rounded-full bg-indigo-900/50 flex items-center justify-center text-indigo-300 text-sm flex-shrink-0 mt-0.5">
                    <i class="fas fa-user"></i>
                </div>
                <div>
                    <div class="flex items-center gap-2 flex-wrap justify-end">
                        <span class="font-medium text-indigo-300 text-sm">You</span>
                        <span class="timestamp">${new Date().toLocaleTimeString([], {hour:'2-digit', minute:'2-digit'})}</span>
                    </div>
                    <div class="mt-1 text-sm leading-relaxed text-gray-200">${escapeHtml(userMessage)}</div>
                </div>
            </div>
        `;
        container.appendChild(userDiv);

        // 入力リセット
        form.reset();
        input.focus();

        // AI 応答 (ダミー: 二十四節気に関連した返答)
        setTimeout(async() => {
            const aiDiv = document.createElement('div');
            aiDiv.className = 'message-ai msg-enter rounded-2xl px-5 py-3 max-w-3xl shadow-sm border border-[#1e293b]';
            const responses = [
                '二十四節気は、中国起源の季節区分で、日本でも広く使われています。立春、雨水、啓蟄、春分、清明、穀雨、立夏、小満、芒種、夏至、小暑、大暑、立秋、処暑、白露、秋分、寒露、霜降、立冬、小雪、大雪、冬至、小寒、大寒の24あります。',
                '「節気」と「中気」が交互に現れ、それぞれ約15日間隔です。現在のカレンダーでも「春分の日」「秋分の日」など祝日として残っています。',
                '旧暦では、中気が含まれない月を「閏月」とする基準でした。そのため、二十四節気は太陰太陽暦の調整に重要な役割を果たしていました。',
                'ちなみに、今日の気候は？ 二十四節気は農作業の目安としても重宝され、現在でも農業や行事と結びついています。'
            ];
            //const randomResponse = responses[Math.floor(Math.random() * responses.length)];
            const data = {
              query: userMessage,
            };
            const answer_str = await sendPostRequest(data);
            console.log("answer_str=", answer_str);
            const randomResponse = answer_str;

            aiDiv.innerHTML = `
                <div class="flex items-start gap-3">
                    <div class="w-7 h-7 rounded-full bg-sky-900/50 flex items-center justify-center text-sky-300 text-sm flex-shrink-0 mt-0.5">
                        <i class="fas fa-robot"></i>
                    </div>
                    <div>
                        <div class="flex items-center gap-2 flex-wrap">
                            <span class="font-medium text-sky-300 text-sm">AI</span>
                            <span class="timestamp">${new Date().toLocaleTimeString([], {hour:'2-digit', minute:'2-digit'})}</span>
                        </div>
                        <div class="mt-1 text-sm leading-relaxed text-gray-300">${escapeHtml(randomResponse)}</div>
                    </div>
                </div>
            `;
            container.appendChild(aiDiv);
            // スクロールを最下部に
            const containerScroll = document.getElementById('message-container');
            containerScroll.scrollTop = containerScroll.scrollHeight;
        }, 500 + Math.random() * 400);
    }
});

// 簡易的なエスケープ
function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

// ページロード後に入力にフォーカス
window.addEventListener('load', function() {
    const input = document.querySelector('.chat-input');
    if (input) input.focus();
    // 既存のメッセージでスクロール調整
    const container = document.getElementById('message-container');
    container.scrollTop = container.scrollHeight;
});

(function() {
})();