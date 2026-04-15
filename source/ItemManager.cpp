/**
 * @file ItemManager.cpp
 * @brief アイテム管理クラスの実装
 */

#include "ItemManager.h"
#include "Config.h"
#include "Player.h"
#include "DxLib.h"

 // コンストラクタ
ItemManager::ItemManager()
    : spawnTimer(0)
{
}

// 初期化
void ItemManager::Init()
{
    spawnTimer = 0;
}

// メイン更新
void ItemManager::Update(Player& player)
{
    // アイテムが存在しない場合、出現タイマーを進める
    if (!powerItem.IsAlive())
    {
        spawnTimer++;

        if (spawnTimer >= ItemConst::SPAWN_INTERVAL)
        {
            SpawnPowerItem();
            spawnTimer = 0;
        }
    }

    // アイテム自体の更新
    powerItem.Update();

    // プレイヤーとの接触判定および効果適用
    if (powerItem.CheckCollision(player))
    {
        if (powerItem.GetType() == ItemType::Power)
        {
            // 無敵化アイテム
            player.ActivatePowerInvincible(ItemConst::POWER_INVINCIBLE_TIME);
        }
        else if (powerItem.GetType() == ItemType::Heal)
        {
            // 回復アイテム
            if (player.GetHP() < PlayerConst::MAX_HP)
            {
                player.RecoverHP(1);
            }
        }
    }
}

// 描画
void ItemManager::Draw(int cameraX, int cameraY) const
{
    powerItem.Draw(cameraX, cameraY);
}

// ランダム出現処理
void ItemManager::SpawnPowerItem()
{
    // マップ範囲内からランダムに座標を決定
    float x = static_cast<float>(GetRand(World::RIGHT - World::LEFT) + World::LEFT);
    float y = static_cast<float>(GetRand(World::BOTTOM - World::TOP) + World::TOP);

    // 種類をランダムに決定
    int randType = GetRand(1);
    ItemType type = (randType == 0) ? ItemType::Power : ItemType::Heal;

    powerItem.Init(x, y, type);
}

// 指定位置出現処理
void ItemManager::SpawnAt(float x, float y, ItemType type)
{
    powerItem.Init(x, y, type);
}