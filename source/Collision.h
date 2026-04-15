/**
 * @file Collision.h
 * @brief 衝突判定ユーティリティ
 * @details 円同士や矩形などの幾何学的な判定処理をまとめたヘッダ
 */

#ifndef COLLISION_H_
#define COLLISION_H_

#include <cmath>

 /**
  * @brief 円同士の衝突判定
  * @param x1, y1 円1の中心座標
  * @param r1     円1の半径
  * @param x2, y2 円2の中心座標
  * @param r2     円2の半径
  * @return 衝突していれば true
  */
inline bool IsCircleCollision(
    float x1, float y1, float r1,
    float x2, float y2, float r2)
{
    // 中心点間の距離の2乗を計算
    float dx = x1 - x2;
    float dy = y1 - y2;
    float distSq = dx * dx + dy * dy;

    // 半径の合計の2乗と比較（ルート計算を避けて高速化）
    float r = r1 + r2;
    return distSq <= r * r;
}

#endif // COLLISION_H_