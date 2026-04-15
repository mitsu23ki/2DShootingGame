/**
 * @file EnemyParam.h
 * @brief 敵の基本パラメータ定義
 */

#ifndef ENEMY_PARAM_H_
#define ENEMY_PARAM_H_

enum EnemyType
{
    ENEMY_FAKE_TANUKI, // 偽物
    ENEMY_REAL_TANUKI, // 本物
    ENEMY_MAX          // 総数
};

struct EnemyParam
{
    int   maxHp;         // 最大HP
    int   attack;        // 攻撃力（弾のダメージなど）
    float speed;         // 移動速度
    int   burstMax;      // 一度の攻撃での連射数
    int   burstInterval; // 連射時の弾間隔（フレーム数）
    int   restInterval;  // 次の攻撃動作に移るまでの待機時間
};

// 外部参照用のパラメータ実体
extern EnemyParam gEnemyParam[ENEMY_MAX];

#endif // ENEMY_PARAM_H_