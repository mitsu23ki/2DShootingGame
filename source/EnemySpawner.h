/**
 * @file EnemySpawner.h
 * @brief 敵の出現（スポーン）管理クラス
 * @details ラウンド数に基づいた敵の生成数や出現位置の計算を行う
 */

#ifndef ENEMY_SPAWNER_H_
#define ENEMY_SPAWNER_H_

class EnemyManager;

class EnemySpawner
{
public:
    // コンストラクタ
    EnemySpawner();

    /**
     * @brief ラウンド開始時の敵生成
     * @param round 現在のラウンド数
     * @param enemyManager 敵を登録する管理クラスへの参照
     */
    void SpawnRoundEnemies(int enemyNum, EnemyManager& enemyManager);

private:
    // 画面内からランダムな座標を取得
    float GetRandomX() const;
    float GetRandomY() const;
};

#endif  // ENEMY_SPAWNER_H_