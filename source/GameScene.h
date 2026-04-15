/**
 * @file GameScene.h
 * @brief ゲームプレイシーンの定義
 * @details 全てのゲームオブジェクトの更新・描画、およびゲーム進行（ラウンド・スコア）を管理する
 */

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "Scene.h"
#include "Player.h"
#include "Camera.h"
#include "Map.h"
#include "EnemyManager.h"
#include "EnemySpawner.h"
#include "RoundManager.h"
#include "LaserSystem.h"
#include "CheckKey.h"
#include "Collision.h"
#include "ScoreManager.h"
#include "Item.h"
#include "ItemManager.h"

class GameScene : public Scene {
private:
    // シーン状態
    SceneType nextScene = SceneType::None; // 次に遷移するシーンの種類

    // プレイヤー
    Player player;                  // プレイヤーオブジェクト
    Camera camera;                  // スクロール制御用カメラ

    // フィールド
    Map map;                        // 背景・地形データ

    // 敵システム
    EnemyManager enemyManager;      // 敵の生存管理・描画
    EnemySpawner enemySpawner;      // 敵の生成ロジック

    // ラウンド管理
    RoundManager roundManager;      // ラウンド数・進行管理
    ScoreManager scoreManager;      // スコア計算・保持

    // 攻撃システム
    LaserSystem laserSystem;        // プレイヤーの攻撃処理

    // アイテム
    Item powerItem;                 // 特定のパワーアップアイテム
    ItemManager itemManager;        // アイテム全体の出現・取得管理

    // 一時状態
    enum class PauseReason {
        None,                       // 通常稼働中
        StartWait,                  // 開始前のカウントダウン等
        CutIn,                      // 特殊演出中
        RoundStart                  // ラウンド開始待機
    };
    PauseReason pauseReason;        // 現在の一時停止理由
    bool isGameStarted = false;     // ゲームループ開始フラグ
    int gameOverTimer;
    bool isGameOverStarted;
    int fadeAlpha;
    float circleRadius;
    // 内部更新ロジック
    bool UpdateGameOver();          // 終了判定（プレイヤー死亡など）
    void UpdatePlayer();            // プレイヤーの行動更新
    void UpdateEnemies();           // 敵群の更新
    void UpdateRoundClearBonus();   // クリア時のボーナス加算
    void UpdateCombat();            // 攻撃判定の橋渡し
    void UpdateCamera();            // カメラの追従更新
    void UpdateRoundFlow();         // ラウンド開始・終了監視

    // その他内部処理
    void CheckCollision();          // 衝突判定一括処理
    void StartRound();              // ラウンド開始セットアップ
   
public:
    GameScene();

    // 基本処理
    void Init() override;
    void Update() override;
    void Draw() override;

    // Getter
    SceneType GetNextScene() const override { return nextScene; }
};

#endif // GAMESCENE_H_