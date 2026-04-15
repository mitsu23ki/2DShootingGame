/**
 * @file EnemySpawner.cpp
 * @brief EnemySpawnerクラスの実装
 */

#include "EnemySpawner.h"
#include "EnemyManager.h"
#include "Config.h"
#include <cstdlib>

 // コンストラクタ
EnemySpawner::EnemySpawner()
{
}

void EnemySpawner::SpawnRoundEnemies(int enemyNum, EnemyManager& enemyManager)
{
    // 本物1体分引く
    int fakeCount = enemyNum - 1;

    // 偽物
    for (int i = 0; i < fakeCount; i++)
    {
        float x = GetRandomX();
        float y = GetRandomY();
        enemyManager.SpawnEnemy(x, y, ENEMY_FAKE_TANUKI);
    }

    // 本物（1体固定）
    float realX = (World::LEFT + World::RIGHT) / 2.0f;
    float realY = (World::TOP + World::BOTTOM) / 2.0f;
    enemyManager.SpawnEnemy(realX, realY, ENEMY_REAL_TANUKI);
}

// ランダムX座標取得（ワールド座標の範囲内）
float EnemySpawner::GetRandomX() const
{
    int range = static_cast<int>(World::RIGHT - World::LEFT);
    if (range <= 0) return World::LEFT;

    int random = rand() % range;
    return static_cast<float>(World::LEFT + random);
}

// ランダムY座標取得（ワールド座標の範囲内）
float EnemySpawner::GetRandomY() const
{
    int range = static_cast<int>(World::BOTTOM - World::TOP);
    if (range <= 0) return World::TOP;

    int random = rand() % range;
    return static_cast<float>(World::TOP + random);
}