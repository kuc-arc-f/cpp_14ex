(function() {
    "use strict";

    // ----- データ管理 -----
    let todos = [];
    const STORAGE_KEY = 'todoAppData';

    // 編集中のTODO ID (null = 新規作成モードではないが、ここでは編集用)
    let editingId = null;

    // DOM 要素
    const addBtn = document.getElementById('addTodoBtn');
    const todoListContainer = document.getElementById('todoListContainer');
    const emptyMessage = document.getElementById('emptyMessage');
    const todoCount = document.getElementById('todoCount');

    // 追加ダイアログ要素
    const addDialog = document.getElementById('addDialog');
    const closeAddDialogBtn = document.getElementById('closeAddDialogBtn');
    const cancelAddBtn = document.getElementById('cancelAddBtn');
    const addTitle = document.getElementById('addTitle');
    //const addStatus = document.getElementById('addStatus');
    const addMemo = document.getElementById('addMemo');
    const addContent = document.getElementById('content');
    const addFoodOrange = document.getElementById('food_orange');
    const addFoodApple = document.getElementById('food_apple');
    const addFoodBanana = document.getElementById('food_banana');
    const addPubDate = document.getElementById('pub_date');
    const addQty1 = document.getElementById('qty1');
    const addQty2 = document.getElementById('qty2');
    const addQty3 = document.getElementById('qty3');
    const registerTodoBtn = document.getElementById('registerTodoBtn');

    // 編集ダイアログ要素
    const dialog = document.getElementById('detailDialog');
    const closeDialogBtn = document.getElementById('closeDialogBtn');
    const editTitle = document.getElementById('editTitle');
    const editStatus = document.getElementById('editStatus');
    const editMemo = document.getElementById('editMemo');
    const editContent = document.getElementById('editContent');
    const editFoodOrange = document.getElementById('editFoodOrange');
    const editFoodApple = document.getElementById('editFoodApple');
    const editFoodBanana = document.getElementById('editFoodBanana');
    const editPubDate = document.getElementById('editPubDate');
    const editQty1 = document.getElementById('editQty1');
    const editQty2 = document.getElementById('editQty2');
    const editQty3 = document.getElementById('editQty3');
    //const saveEditBtn = document.getElementById('saveEditBtn');
    const deleteTodoBtn = document.getElementById('deleteTodoBtn');

    // ----- 初期化 / ローカルストレージ読み込み -----
    async function getData() {
        const res = await fetch('/api/todo/list');
        const data = await res.json();
        return data;
    }    
    async function loadTodos() {
        try {
            const list_data = await getData();
            console.log(list_data)
            todos = list_data;
            // 古いデータにmemoが無い場合に備える
            //todos = todos.map(t => ({ ...t, memo: t.memo || '' }));
        } catch (e) {
            todos = [];
        }
        //if (stored) {
        //}
        renderTodos();
    }

    function saveTodos() {
        localStorage.setItem(STORAGE_KEY, JSON.stringify(todos));
        updateCount();
    }

    // ----- 描画 -----
    function renderTodos() {
        if (todos.length === 0) {
            todoListContainer.innerHTML = `
                <p class="text-gray-400 text-sm text-center py-8" id="emptyMessage">
                    <i class="fas fa-inbox mr-2"></i> TODOがありません。追加してみましょう！
                </p>
            `;
            updateCount();
            return;
        }

        let html = '';
        todos.forEach(todo => {
            // ステータスに応じたバッジクラス
            let badgeClass = 'badge-pending';
            let statusLabel = '未着手';
            if (todo.status === '作業中') { badgeClass = 'badge-working'; statusLabel = '作業中'; }
            else if (todo.status === '緊急') { badgeClass = 'badge-urgent'; statusLabel = '緊急'; }
            else if (todo.status === '完了') { badgeClass = 'badge-pending'; statusLabel = '完了'; } // 完了はグレー

            // エスケープ処理 (簡易)
            const safeTitle = escapeHtml(todo.title);
            const safeMemo = escapeHtml(todo.memo || '');

            html += `
                <div class="todo-row bg-gray-50 border border-gray-200 rounded-xl px-4 py-3 flex items-center justify-between transition hover:shadow-sm" data-id="${todo.id}">
                    <div class="flex items-center gap-3 flex-1 min-w-0">
                        <span class="text-gray-700 font-medium truncate">${safeTitle}</span>
                        <span class="badge ${badgeClass} flex-shrink-0"># ${statusLabel}</span>
                    </div>
                    <div class="flex items-center gap-2 flex-shrink-0 ml-2">
                        <button class="edit-btn text-blue-500 hover:text-blue-700 transition text-sm px-2 py-1 rounded-full hover:bg-blue-50" data-id="${todo.id}">
                            <i class="fas fa-pencil"></i> 編集
                        </button>
                    </div>
                </div>
            `;
        });

        todoListContainer.innerHTML = html;

        // ----- イベントリスナー: 行クリック (詳細表示) -----
        document.querySelectorAll('.todo-row').forEach(row => {
            row.addEventListener('click', function(e) {
                // 編集ボタンをクリックした場合は、行クリックを無効化しないようにするが、
                // 編集ボタンには stopPropagation を仕込むので、ここではそのまま。
                const id = parseInt(this.dataset.id);
                openDetailDialog(id);
            });
        });

        // ----- イベントリスナー: 編集ボタン (行内の編集ボタン) -----
        document.querySelectorAll('.edit-btn').forEach(btn => {
            btn.addEventListener('click', function(e) {
                e.stopPropagation(); // 行クリックとの競合を防ぐ
                const id = parseInt(this.dataset.id);
                openDetailDialog(id);
            });
        });

        updateCount();
    }

    // シンプルなエスケープ（XSS対策）
    function escapeHtml(text) {
        if (!text) return '';
        const div = document.createElement('div');
        div.textContent = text;
        return div.innerHTML;
    }

    // 件数更新
    function updateCount() {
        const count = todos.length;
        todoCount.textContent = count + '件';
        if (count === 0) {
            // emptyMessage は既にrender内で表示しているが、念のため
        }
    }

    // ----- ダイアログ操作 -----
    function openDetailDialog(id) {
        const todo = todos.find(t => t.id === id);
        if (!todo) return;
        console.log(todo);

        editingId = id;
        editTitle.value = todo.title || '';
        editStatus.value = todo.status || '未着手';
        editMemo.value = todo.memo || '';
        editContent.value = todo.content || '';
        const editPublicChecked = document.querySelector('input[name="editPublic"][value="' + (todo.public || '公開') + '"]');
        if (editPublicChecked) editPublicChecked.checked = true;
        editFoodOrange.checked = !!todo.food_orange;
        editFoodApple.checked = !!todo.food_apple;
        editFoodBanana.checked = !!todo.food_banana;
        editPubDate.value = todo.pub_date || '';
        editQty1.value = todo.qty1;
        editQty2.value = todo.qty2;
        editQty3.value = todo.qty3;

        dialog.classList.remove('hidden');
        // フォーカスをタイトルに
        setTimeout(() => editTitle.focus(), 100);
    }

    function closeDialog() {
        dialog.classList.add('hidden');
        editingId = null;
        // フォームをクリア（次回開くときに上書きされるが、念のため）
        editTitle.value = '';
        editStatus.value = '未着手';
        editMemo.value = '';
        editContent.value = '';
        editFoodOrange.checked = false;
        editFoodApple.checked = false;
        editFoodBanana.checked = false;
        editPubDate.value = '';
        editQty1.value = '';
        editQty2.value = '';
        editQty3.value = '';
    }

    // ----- ダイアログ内イベント -----
    // 保存
    function saveEdit() {
        if (editingId === null) {
            // 新規作成モードはないが、一応
            return;
        }
        const title = editTitle.value.trim();
        if (!title) {
            alert('タイトルを入力してください。');
            return;
        }

        const todoIndex = todos.findIndex(t => t.id === editingId);
        if (todoIndex === -1) return;

        // 更新
        todos[todoIndex].title = title;
        todos[todoIndex].status = editStatus.value;
        todos[todoIndex].memo = editMemo.value.trim();
        todos[todoIndex].content = editContent.value.trim();
        const editPublicRadio = document.querySelector('input[name="editPublic"]:checked');
        todos[todoIndex].public = editPublicRadio ? editPublicRadio.value : '公開';
        todos[todoIndex].food_orange = editFoodOrange.checked;
        todos[todoIndex].food_apple = editFoodApple.checked;
        todos[todoIndex].food_banana = editFoodBanana.checked;
        todos[todoIndex].pub_date = editPubDate.value;
        todos[todoIndex].qty1 = editQty1.value.trim();
        todos[todoIndex].qty2 = editQty2.value.trim();
        todos[todoIndex].qty3 = editQty3.value.trim();

        saveTodos();
        renderTodos();
        closeDialog();
    }
    async function deletePost(data) {
        try {
            const response = await fetch('/api/todo/delete', {
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
            console.log("result=", result)
            return result;
        } catch (error) {
            console.error('エラーが発生しました:', error);
        }
    } 
    // 削除
    async function deleteTodo() {
        if (editingId === null) return;
        if (!confirm('このTODOを削除してもよろしいですか？')) return;
        const data = {id: Number(editingId)}
        await deletePost(data)
        todos = todos.filter(t => t.id !== editingId);
        saveTodos();
        renderTodos();
        closeDialog();
    }

    // ----- 追加ダイアログ -----
    function openAddDialog() {
        addTitle.value = '';
        //addStatus.value = '未着手';
        addMemo.value = '';
        addContent.value = '';
        document.querySelector('input[name="public"][value="公開"]').checked = true;
        addFoodOrange.checked = false;
        addFoodApple.checked = false;
        addFoodBanana.checked = false;
        addPubDate.value = '';
        addQty1.value = 0;
        addQty2.value = 0;
        addQty3.value = 0;
        addDialog.classList.remove('hidden');
        setTimeout(() => addTitle.focus(), 100);
    }

    function closeAddDialog() {
        addDialog.classList.add('hidden');
        addTitle.value = '';
        //addStatus.value = '未着手';
        addMemo.value = '';
    }

    async function sendPostCreate(data) {
        try {
            const response = await fetch('/api/todo/create', {
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
            console.log('成功:', result);
            return result;
        } catch (error) {
            console.error('エラーが発生しました:', error);
        }
    }    
    async function registerNewTodo() {
        const title = addTitle.value.trim();
        if (!title) {
            alert('タイトルを入力してください。');
            addTitle.focus();
            return;
        }

        const publicRadio = document.querySelector('input[name="public"]:checked');
        const newData = {
            title: title,
            content: addContent.value.trim(),
            public: publicRadio ? 0 : 1,
            food_orange: addFoodOrange.checked ? 0 : 1,
            food_apple: addFoodApple.checked ? 0 : 1,
            food_banana: addFoodBanana.checked ? 0 : 1,
            pub_date: addPubDate.value,
            qty1: Number(addQty1.value.trim()),
            qty2: Number(addQty2.value.trim()),
            qty3: Number(addQty3.value.trim()),
        }
        await sendPostCreate(newData);
        loadTodos();
        renderTodos();
        closeAddDialog();
    }

    // ----- イベント登録 -----
    addBtn.addEventListener('click', openAddDialog);
    closeAddDialogBtn.addEventListener('click', closeAddDialog);
    cancelAddBtn.addEventListener('click', closeAddDialog);
    registerTodoBtn.addEventListener('click', registerNewTodo);
    addDialog.addEventListener('click', function(e) {
        if (e.target === this) {
            closeAddDialog();
        }
    });
    addTitle.addEventListener('keydown', function(e) {
        if (e.key === 'Enter') {
            e.preventDefault();
            registerNewTodo();
        }
    });

    // ダイアログ閉じる
    closeDialogBtn.addEventListener('click', closeDialog);
    // オーバーレイクリックで閉じる（ダイアログの外側）
    dialog.addEventListener('click', function(e) {
        if (e.target === this) {
            closeDialog();
        }
    });
    // ESCキーで閉じる
    document.addEventListener('keydown', function(e) {
        if (e.key !== 'Escape') return;
        if (!addDialog.classList.contains('hidden')) {
            closeAddDialog();
            return;
        }
        if (!dialog.classList.contains('hidden')) {
            closeDialog();
        }
    });

    // 保存ボタン
    //saveEditBtn.addEventListener('click', saveEdit);
    // 削除ボタン
    deleteTodoBtn.addEventListener('click', deleteTodo);

    // ダイアログ内のフォームでもEnterで保存できるように（ただしtextareaでは改行を許可）
    editTitle.addEventListener('keydown', function(e) {
        if (e.key === 'Enter') {
            e.preventDefault();
            saveEdit();
        }
    });
    // statusやmemoはあえてEnterでの保存は行わない（誤操作防止）

    // ----- 起動 -----
    loadTodos();

})();
