/**
 * @file EnemyManager.h
 * @brief 敵管理クラスの定義
 * @details 敵および敵弾の生成、更新、描画、衝突判定、ボスイベントを一括管理する
 */

#ifndef ENEMY_MANAGER_H_
#define ENEMY_MANAGER_H_

#include <vector>
#include <functional>
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"

class ScoreManager;

class EnemyManager {
private:
    // 敵・弾管理
    std::vector<Enemy> enemies;               // 出現中の敵リスト
    std::vector<Bullet> bullets;              // 画面上の敵弾リスト

    // ボスイベント状態
    bool realTanukiDeadTrigger;               // 本物を倒した瞬間のフラグ
    bool isCutInPlaying;                      // カットイン演出中か
    bool hasPlayedCutIn;                      // カットインを既に再生したか
    int cutInTimer;                           // カットインの表示残り時間

    // 画像リソース
    int imgIdle;                              // 偽物の待機画像
    int imgWalk[2];                           // 偽物の歩行画像
    int imgIdleDamage;                        // 偽物の被弾画像
    int imgWalkDamage[2];                     // 偽物の被弾歩行画像
    int imgIdleReal;                          // 本物の待機画像
    int imgWalkReal[2];                       // 本物の歩行画像
    int imgSmoke;                             // 撃破時の煙画像
    int imgCutIn;                             // ボス撃破のカットイン画像
    int imgBurning;                           // ボス撃破の燃焼画像

    // 内部処理
    void UpdateEnemies(float playerX, float playerY); // 敵の移動やAIの更新
    void UpdateBullets();                     // 弾の移動と消滅チェック
    void CheckBulletCollision(Player& player); // 弾とプレイヤーの当たり判定
    void AddScore(ScoreManager& scoreManager); // 敵撃破時のスコア加算
    void CheckBossEvent();                    // ボス撃破時の演出開始チェック
    void RemoveDeadObjects();                 // 死亡した敵や弾をリストから削除

    // スコアポップ表示用
    struct ScorePopup
    {
        float x, y; int score; int timer;
    };    // 取得スコアの浮遊表示データ
    std::vector<ScorePopup> scorePopups;      // 表示中のスコアポップリスト
    void UpdateScorePopups();                 // スコアポップの移動と寿命更新

public:
    // 生成 / 破棄
    EnemyManager();                           // コンストラクタ
    ~EnemyManager();                          // デストラクタ

    // 初期化
    void Init();                              // リソース読み込みと変数リセット

    // 更新 
    void Update(                              // 敵関連の全更新処理をまとめる
        float playerX,
        float playerY,
        float playerRadius,
        Player& player,
        ScoreManager& scoreManager);
    
    // 描画
    void Draw(int cameraX, int cameraY) const; // 敵・弾・ポップアップを描画
    void DrawCutIn() const;                   // カットイン演出を最前面に描画

    // 敵操作
    void SpawnEnemy(float x, float y, EnemyType type); // 指定した種類で敵を生成
    void SpawnOneFake(float x, float y);      // 偽物のタヌキを1体生成
    void Clear();                             // 全ての敵と弾を消去

    // 状態取得
    int GetEnemyCount() const;                // 現在の敵の総数を返す
    const std::vector<Enemy>& GetEnemies() const; // 敵リストの参照（読み取り用）
    const std::vector<Bullet>& GetBullets() const; // 弾リストの参照（読み取り用）
    bool IsCutInPlaying() const;              // 演出中につき操作を止める判定などに使用
    std::vector<Enemy>& GetEnemies();         // 敵リストの参照（書き換え用）
    std::vector<bool> enemyCounted;           // 二重カウント防止用の状態保存
    std::function<void(EnemyType)> onEnemyKilled; // 敵撃破時に外部へ通知するコールバック

    // ボス撃破イベント
    bool IsRealTanukiDefeated();              // 本物が倒されたかチェック
    void DrawScorePopups(int cameraX, int cameraY) const; // スコアポップを描画
};

#endif