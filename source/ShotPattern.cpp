/**
 * @file ShotPattern.cpp
 * @brief 敵の弾幕パターン生成関数の実装
 */

#include "ShotPattern.h"
#include <cmath>
#include "DxLib.h"

 // プレイヤー狙撃弾
void ShotPattern::FireAimed(
    std::vector<Bullet>& bullets,
    float x, float y,
    float targetX, float targetY,
    float speed)
{
    // ターゲットへのベクトルを計算
    float dx = targetX - x;
    float dy = targetY - y;

    // ベクトルの正規化（長さを1にする）
    float len = sqrtf(dx * dx + dy * dy);
    if (len <= 0) return;

    dx /= len;
    dy /= len;

    // 弾を追加
    bullets.emplace_back(
        x, y,
        dx, dy,
        speed,
        BULLET_SMALL
    );
}

// 全方向弾
void ShotPattern::FireRadial(
    std::vector<Bullet>& bullets,
    float x, float y,
    int count,
    float speed)
{
    for (int i = 0; i < count; i++)
    {
        // 円周を分割する角度を計算
        float angle = i * (2.0f * DX_PI_F / count);

        // 角度から進行方向ベクトルを算出
        float dx = cosf(angle);
        float dy = sinf(angle);

        // 弾を追加
        bullets.emplace_back(
            x, y,
            dx, dy,
            speed,
            BULLET_BIG
        );
    }
}