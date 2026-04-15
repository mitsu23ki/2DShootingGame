/**
 * @file LaserSystem.h
 * @brief プレイヤーのレーザー攻撃システムの定義
 */

#ifndef LASER_SYSTEM_H_
#define LASER_SYSTEM_H_

#include "EnemyManager.h"
#include "Player.h"

class LaserSystem {
private:
    // ターゲット情報
    Enemy* targetEnemy;     // 現在ロックしている敵
    float  nearestDist;     // プレイヤーと敵の距離

    // 制御用タイマー
    int animCounter;        // レーザー演出の更新用
    int damageTimer;        // ダメージ発生の間隔管理

public:
    LaserSystem();

    // メインループ
    void Update(Player& player, EnemyManager& enemyManager); // 敵探索・射程判定・ダメージ
    void Draw(Player& player, int cameraX, int cameraY) const; // レーザー描画
};

#endif // LASER_SYSTEM_H_