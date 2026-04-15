/**
 * @file EnemyParam.cpp
 * @brief 敵パラメータの実体定義
 */

#include "EnemyParam.h"

EnemyParam gEnemyParam[ENEMY_MAX] =
{
    // [ENEMY_FAKE_TANUKI] 偽物タヌキ
    {
        2,      // maxHp
        1,      // attack
        1.5f,   // speed
        3,      // burstMax
        10,     // burstInterval
        320     // restInterval
    },

    // [ENEMY_REAL_TANUKI] 本物タヌキ
    {
        3,      // maxHp
        5,      // attack
        4.0f,   // speed
        1,      // burstMax
        0,      // burstInterval
        0       // restInterval
    }
};