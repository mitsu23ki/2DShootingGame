/**
 * @file LaserSystem.cpp
 * @brief プレイヤーのレーザー攻撃システムの実装
 */

#include "LaserSystem.h"
#include "Config.h"
#include "DxLib.h"
#include <cmath>

 // コンストラクタ
LaserSystem::LaserSystem()
    : targetEnemy(nullptr)
    , nearestDist(0.0f)
    , animCounter(0)
    , damageTimer(0)
{
}

// 更新処理
void LaserSystem::Update(Player& player, EnemyManager& enemyManager)
{
    animCounter++;

    // ターゲット探索の初期化
    targetEnemy = nullptr;
    nearestDist = 999999.0f;

    auto& enemies = enemyManager.GetEnemies();

    // 最も近い有効な敵を探索
    for (auto& enemy : enemies) {
        // 死亡中、またはターゲット不可の敵は無視
        if (enemy.IsDying() || !enemy.CanBeTargeted()) continue;

        float dist = std::hypot(player.GetX() - enemy.GetX(), player.GetY() - enemy.GetY());

        // 最小距離を更新してターゲットを確定
        if (dist < nearestDist) {
            nearestDist = dist;
            targetEnemy = &enemy;
        }
    }

    // ターゲットが存在し、かつ射程内の場合
    if (targetEnemy != nullptr && nearestDist < Laser::RANGE)
    {
        damageTimer++;

        // 一定間隔ごとにダメージを与える
        if (damageTimer >= Laser::DAMAGE_INTERVAL)
        {
            targetEnemy->Damage(Laser::DAMAGE);
            damageTimer = 0;
        }
    }
    else
    {
        damageTimer = 0;
    }
}

// 描画処理
void LaserSystem::Draw(Player& player, int cameraX, int cameraY) const
{
    // 描画対象がいない、死亡中、または射程外なら終了
    if (targetEnemy == nullptr) return;
    if (targetEnemy->IsDying()) return;
    if (nearestDist >= Laser::RANGE) return;

    // スクリーン座標への変換
    int px = static_cast<int>(player.GetX()) - cameraX;
    int py = static_cast<int>(player.GetY()) - cameraY;

    int ex = static_cast<int>(targetEnemy->GetX()) - cameraX;
    int ey = static_cast<int>(targetEnemy->GetY()) - cameraY;

    // 太い外光と細い芯を重ねてレーザーを表現
    DrawLine(px, py, ex, ey, GetColor(255, 80, 80), 8);  // 外側の光
    DrawLine(px, py, ex, ey, GetColor(255, 255, 255), 2); // 内側のレーザー芯
}