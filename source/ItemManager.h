/**
 * @file ItemManager.h
 * @brief アイテム管理クラスの定義
 */

#ifndef ITEM_MANAGER_H_
#define ITEM_MANAGER_H_

#include "Item.h"

class Player;

class ItemManager {
private:
    // メンバ変数
    Item powerItem;     // 管理するアイテム単体（現在は1つ）
    int  spawnTimer;    // 出現までのカウントダウン用タイマー

public:
    ItemManager();

    // メインループ
    void Init();
    void Update(Player& player);
    void Draw(int cameraX, int cameraY) const;

    // アイテム生成
    void SpawnAt(float x, float y, ItemType type); // 指定位置に生成

private:
    // 内部ロジック
    void SpawnPowerItem(); // ランダムな位置にアイテムを生成
};

#endif // ITEM_MANAGER_H_