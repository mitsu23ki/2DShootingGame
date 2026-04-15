/**
 * @file Item.cpp
 * @brief アイテムクラスの実装
 */

#include "Item.h"
#include "Player.h"
#include "DxLib.h"
#include <cmath>

 // コンストラクタ
Item::Item()
    : x(0), y(0)
    , radius(24.0f)
    , img(-1)
    , isAlive(false)
    , type(ItemType::Power)
{
}

// 初期化
void Item::Init(float spawnX, float spawnY, ItemType itemType)
{
    x = spawnX;
    y = spawnY;
    type = itemType;

    // 種類に応じた画像の読み込み
    if (type == ItemType::Power)
        img = LoadGraph("Data/power.png");
    else
        img = LoadGraph("Data/heal.png");

    isAlive = true;
}

// 更新
void Item::Update()
{
    // 現在は移動等の処理なし
}

// 描画
void Item::Draw(int cameraX, int cameraY) const
{
    if (!isAlive) return;

    // 中心座標から描画位置（左上）へオフセット
    int drawX = static_cast<int>(x) - cameraX - 32;
    int drawY = static_cast<int>(y) - cameraY - 32;

    DrawGraph(drawX, drawY, img, TRUE);

#ifdef DEBUG
    // 当たり判定の可視化
    DrawCircle(drawX + 32, drawY + 32, static_cast<int>(radius), GetColor(255, 0, 0), FALSE);
#endif
}

// 生存確認
bool Item::IsAlive() const
{
    return isAlive;
}

// アイテムの削除
void Item::Kill()
{
    isAlive = false;
}

// 種類取得
ItemType Item::GetType() const
{
    return type;
}

// プレイヤーとの接触判定
bool Item::CheckCollision(const Player& player)
{
    if (!isAlive) return false;

    // 距離の計算
    float dx = x - player.GetX();
    float dy = y - player.GetY();
    float dist = std::hypot(dx, dy);

    // 半径の合計値による円判定
    if (dist < radius + player.GetRadius())
    {
        isAlive = false;
        return true;
    }

    return false;
}