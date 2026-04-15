/**
 * @file SceneManager.h
 * @brief シーン遷移を管理するクラスの定義
 */

#include "Scene.h"
#include "Title.h"
#include "GameScene.h"

class SceneManager {
private:
    // メンバ変数
    Scene* currentScene; // 現在実行中のシーンオブジェクト

public:
    // コンストラクタ
    SceneManager() : currentScene(nullptr) {}

    // デストラクタ
    ~SceneManager() {
        delete currentScene; // メモリ解放
    }

    // シーン切り替え処理
    void ChangeScene(SceneType type) {
        // 現在のシーンがあれば破棄
        if (currentScene != nullptr) {
            delete currentScene;
        }

        // 新しいシーンを動的生成
        switch (type) {
        case SceneType::Title: currentScene = new TitleScene(); break;
        case SceneType::Game:  currentScene = new GameScene();  break;
            // case SceneType::Menu: currentScene = new MenuScene(); break; // 将来追加用
        }

        // 生成したシーンの初期化
        if (currentScene != nullptr) {
            currentScene->Init();
        }
    }

    // メイン更新
    void Update() {
        if (currentScene == nullptr) return;

        // 現在のシーンを更新
        currentScene->Update();

        // シーン側から遷移リクエストがあるかチェック
        SceneType next = currentScene->GetNextScene();
        if (next != SceneType::None) {
            ChangeScene(next); // 次のシーンへ
        }
    }

    // 描画
    void Draw() {
        if (currentScene != nullptr) {
            currentScene->Draw();
        }
    }
};