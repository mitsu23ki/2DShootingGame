/**
 * @file Scene.h
 * @brief シーン管理の基底クラス定義
 */

#ifndef SCENE_H_
#define SCENE_H_

 // シーンの種類
enum class SceneType {
    None,
    Title,      // タイトル
    Menu,       // メニュー
    Game,       // スコアアタック
    GameOver,   // スコアランキング
    HowToPlay,  // ゲーム説明画面
    Config,     // 音量設定
    Tutorial    // チュートリアル
};

class Scene {
public:
    virtual ~Scene() {}

    // 抽象メソッド（派生クラスで実装）
    virtual void Init() = 0;   // 初期化処理
    virtual void Update() = 0; // 更新処理
    virtual void Draw() = 0;   // 描画処理

    // 遷移判定
    virtual SceneType GetNextScene() const = 0; // 次に遷移すべきシーンを返す
};

#endif // SCENE_H_