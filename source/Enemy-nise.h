/******************************************************************************
 * @file    Enemy.h
 * @brief   敵キャラクタークラス
 ******************************************************************************/
#ifndef ENEMY_H_
#define ENEMY_H_

#include "EnemyParam.h"
#include <vector>
#include "Bullet.h"

class Enemy
{
private:
    // ===== 基本情報 =====
    EnemyType type;     // 敵の種類

    // ===== 位置・移動 =====
    float x;
    float y;
    float dirX;         // 移動方向X
    float dirY;         // 移動方向Y
    float speed;        // 移動速度
    int wanderTimer;    // 方向変更まで

    float targetX;      // 目的地X（本物用）
    float targetY;      // 目的地Y

    // ===== ステータス =====
    int hp;
    bool dead;

    // ===== 当たり判定 =====
    float radius;

    // ===== 画像 =====
    int imgIdle;
    int imgWalk[2];

    int imgIdleDamage;
    int imgWalkDamage[2];

    int imgIdleReal;
    int imgWalkReal[2];

    int imgSmoke;

    int width;
    int height;

    // ===== アニメーション =====
    int animCounter;

    // ===== ダメージ演出 =====
    bool isDamaged;
    int damageTimer;

    // ===== HPバー =====
    int hpBarTimer;

    // ===== 死亡演出 =====
    bool dying;
    int deathTimer;

    // ===== 射撃 =====
    int shootTimer = 0;
    int burstCount = 0;
    int burstMax = 1;

    int burstInterval = 10; // 発射間隔
    int restInterval = 320; // 休憩

    bool hasFiredThisRun = false; // 横断中に撃ったか
    bool canBeTargeted = true;

    // ===== 内部処理 =====
    void Move();                    // 移動
    void ChangeDirection();         // 徘徊方向変更
    void SetRandomOffscreenTarget();// 画面外目標
    void AvoidOthers(const std::vector<Enemy>& allEnemies); // 回避
    // ===== 更新内部 =====
    void UpdateDamage();     // 被ダメ処理
    void UpdateHPBar();      // HPバー処理
    bool UpdateDeath();      // 死亡演出（trueで終了）
    void UpdateAI(
        const std::vector<Enemy>& allEnemies,
        std::vector<Bullet>& bullets,
        float playerX,
        float playerY);      // 行動処理
    void UpdateAnimation();  // アニメ更新
public:
    // ===== 生成 / 破棄 =====
    Enemy(float startX, float startY, EnemyType type);
    ~Enemy();

    // ===== 更新 / 描画 =====
    void Update(
        const std::vector<Enemy>& allEnemies,
        std::vector<Bullet>& bullets,
        float playerX,
        float playerY);

    void Draw(int cameraX, int cameraY) const;

    // ===== 初期設定 =====
    void SetImages(int idle, int walk[2]);
    void SetDamageImages(int idle, int walk[2]);
    void SetRealImages(int idle, int walk[2]);
    void SetSmokeImage(int img);

    // ===== 状態取得 =====
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetRadius() const { return radius; }

    EnemyType GetType() const { return type; }

    bool CanBeTargeted() const
    {
        return !dying && !dead && canBeTargeted;
    }

    bool IsDead() const { return dead; }
    bool IsDying() const { return dying; }

    int GetDeathTimer() const { return deathTimer; }

    // ===== 強制燃焼（全滅演出） =====
    void ForceBurn(int burnImg)
    {
        if (!dying && !dead)
        {
            dying = true;
            deathTimer = 180;   // 演出時間
            imgSmoke = burnImg; // 炎画像
        }
    }

    // ===== ダメージ =====
    void Damage(int amount);
};

#endif