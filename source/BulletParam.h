/**
 * @file BulletParam.h
 * @brief 弾の性能定義
 */

#ifndef BULLET_PARAM_H_
#define BULLET_PARAM_H_

#include "Bullet.h"

 /**
  * @brief 弾の静的パラメータ構造体
  * @details 全ての弾インスタンスで共有される設定値
  */
struct BulletParam
{
    float radius;        // 当たり判定の半径
    unsigned int color;  // 描画色（DxLibの GetColor で生成した値）
};

// 弾の種類数（BULLET_MAX）に合わせて実体を宣言
extern BulletParam gBulletParam[BULLET_MAX];

#endif // BULLET_PARAM_H_