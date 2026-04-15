/**
 * @file Item.h
 * @brief アイテムクラスの定義
 */

#ifndef ITEM_H_
#define ITEM_H_

class Player;

// アイテムの種類
enum class ItemType {
    Power,  // 無敵化アイテム
    Heal    // HP回復アイテム
};

class Item {
private:
    // 基本ステータス
    ItemType type;      // アイテムの種類
    float x;            // X座標
    float y;            // Y座標
    float radius;       // 当たり判定の半径
    int   img;          // 画像ハンドル
    bool  isAlive;      // 生存フラグ

public:
    Item();

    // メインループ
    void Init(float spawnX, float spawnY, ItemType itemType);
    void Update();
    void Draw(int cameraX, int cameraY) const;

    // 判定・情報取得
    bool IsAlive() const;
    void Kill();
    ItemType GetType() const;
    bool CheckCollision(const Player& player); // プレイヤーとの接触判定
};

#endif // ITEM_H_