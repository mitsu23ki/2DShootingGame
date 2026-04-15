/**
 * @file Bullet.cpp
 * @brief Bulletクラスの実装
 */

#include "Bullet.h"
#include "BulletParam.h"
#include "DxLib.h"
#include "Config.h"

 /**
  * @brief 初期化
  * @details BulletParam から種類に応じた当たり判定半径などを取得する
  */
Bullet::Bullet(float startX, float startY,
    float dx, float dy,
    float spd,
    BulletType t)
    : x(startX)
    , y(startY)
    , dirX(dx)
    , dirY(dy)
    , speed(spd)
    , dead(false)
    , type(t)
{
    // グローバルなパラメータ配列から半径を取得
    radius = gBulletParam[type].radius;
}

/**
 * @brief 更新
 * @details 座標を更新し、ワールドの境界線を超えたら死亡フラグを立てる
 */
void Bullet::Update()
{
    // 現在の方向に速度を乗算して移動
    x += dirX * speed;
    y += dirY * speed;

    // ワールド座標の外に出たら自動的に消滅
    if (x < World::LEFT || x > World::RIGHT ||
        y < World::TOP || y > World::BOTTOM)
    {
        dead = true;
    }
}

/**
 * @brief 描画
 * @details パラメータで指定された色で円を描画する
 */
void Bullet::Draw(int cameraX, int cameraY) const
{
    // タイプに応じた色を取得
    int color = gBulletParam[type].color;

    // カメラ座標を差し引いて画面上の位置を計算
    DrawCircle(
        static_cast<int>(x) - cameraX,
        static_cast<int>(y) - cameraY,
        static_cast<int>(radius),
        color,
        TRUE // 塗りつぶし
    );
}