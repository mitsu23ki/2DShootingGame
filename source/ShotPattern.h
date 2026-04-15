/**
 * @file ShotPattern.h
 * @brief 敵の弾幕パターン生成関数の定義
 */

#ifndef SHOT_PATTERN_H_
#define SHOT_PATTERN_H_

#include <vector>
#include "Bullet.h"

namespace ShotPattern {
    // プレイヤー狙撃弾
    void FireAimed(
        std::vector<Bullet>& bullets, // 弾リスト
        float x, float y,             // 発射位置
        float targetX, float targetY, // ターゲット座標
        float speed);                 // 弾速

    // 全方向（放射状）弾
    void FireRadial(
        std::vector<Bullet>& bullets, // 弾リスト
        float x, float y,             // 発射位置
        int count,                    // 弾数
        float speed);                 // 弾速
}

#endif // SHOT_PATTERN_H_