/**
 * @file Bullet.h
 * @brief 敵が発射する弾クラスの定義
 */

#ifndef BULLET_H_
#define BULLET_H_

 /**
  * @brief 弾の種類
  */
enum BulletType
{
    BULLET_SMALL, // 標準の弾
    BULLET_BIG,   // 巨大な弾（当たり判定大）
    BULLET_FAST,  // 高速な弾
    BULLET_MAX    // 種類の総数
};

class Bullet
{
private:
    // ===== 物理・状態属性 =====
    float x, y;        // 現在の座標
    float dirX, dirY;  // 移動方向ベクトル（正規化済み）
    float speed;       // 移動速度
    float radius;      // 当たり判定の半径
    bool  dead;        // 消滅フラグ（trueで削除対象）
    BulletType type;   // 弾のタイプ

public:
    /**
     * @brief コンストラクタ
     * @param startX 出現X座標
     * @param startY 出現Y座標
     * @param dx 移動方向X
     * @param dy 移動方向Y
     * @param spd 移動速度
     * @param t 弾の種類
     */
    Bullet(float startX, float startY,
        float dx, float dy,
        float spd,
        BulletType t);

    // 更新処理（移動・画面外判定）
    void Update();

    // 描画処理
    void Draw(int cameraX, int cameraY) const;

    // 座標・判定の取得
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetRadius() const { return radius; }

    // フラグ操作
    void Kill() { dead = true; }
    bool IsDead() const { return dead; }
};

#endif // BULLET_H_