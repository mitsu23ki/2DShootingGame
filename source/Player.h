/**
 * @file Player.h
 * @brief プレイヤークラスの定義
 * @details 自機の移動、描画、当たり判定、ステータス管理を担う
 */

#ifndef PLAYER_H_
#define PLAYER_H_

struct HitRect {
    int left, top, right, bottom;
};

class Player {
private:
    // 座標・移動
    float playerX;      // 現在のX座標
    float playerY;      // 現在のY座標
    float targetX;      // 移動目的地のX座標
    float targetY;      // 移動目的地のY座標
    float speed;        // 移動速度
    bool  isMoving;     // 移動中フラグ
    bool  isFlip;       // 左右反転フラグ

    // アセット・描画
    int imgIdle;        // 待機中画像ハンドル
    int imgWalk[2];     // 歩行アニメーション画像ハンドル
    int animCounter;    // アニメーション制御用カウンタ
    int imgDead;        // 倒れ画像
    bool isDeadPlayed;  // SE再生済みフラグ
    // ステータス・無敵管理
    int  hp;                   // 現在の体力
    int  invincibleTimer;      // 被弾後の無敵タイマー
    int  powerInvincibleTimer; // アイテムによる無敵タイマー
    bool isStartInvincible;    // 開始時演出用の無敵フラグ

    // チュートリアル
    bool hasMovedOnce;         // 初回移動完了フラグ

    // 当たり判定
    float radius;              // 円形当たり判定の半径

    // 内部更新ロジック
    void HandleInput(int cameraX, int cameraY);
    void Move();
    void CheckWorldBounds();   // マップ外への移動制限
    void UpdateInvincible();   // 各種無敵タイマーの更新
    void UpdateAnimation();

    // 描画処理
    void DrawPlayer(int cameraX, int cameraY) const;      // プレイヤー本体を描画
    void DrawTargetMarker(int cameraX, int cameraY) const; // 移動先のクリック地点を描画
    void DrawHP() const;                                  // 体力（ハート）のUI全体を描画
    void DrawHeart(int x, int y, unsigned int color) const; // ハートを1個描画する

public:
    Player();  // コンストラクタ（初期座標やステータスの設定）
    ~Player(); // デストラクタ（読み込んだ画像の解放）

    // メインループ
    void Init();                               // 画像のロードや判定の初期設定
    void Update(int cameraX, int cameraY);     // 入力、移動、アニメーションの更新
    void Draw(int cameraX, int cameraY) const; // プレイヤーに関連する全ての描画を実行

    // Getter
    HitRect GetHitRect() const;         // 四角形の当たり判定の範囲を返す
    float GetX() const { return playerX; } // 現在のX座標を返す
    float GetY() const { return playerY; } // 現在のY座標を返す
    float GetRadius() const;            // 円判定に使う半径を返す
    int GetHP() const;                  // 現在の残り体力を返す
    bool IsDead() const;                // 死亡状態かどうかを判定する
    bool HasMoved() const;              // 一度でも移動操作を行ったかチェック
    bool IsInvincible() const;          // 無敵（ダメージ・アイテム両方）か判定

    // Setter / Action
    void OnDamage();                         // 被弾時の体力減少と無敵付与
    void RecoverHP(int amount);              // 指定量だけ体力を回復する
    void ActivatePowerInvincible(int time);  // 無敵アイテムによる無敵を開始
    void SetInvincible(int time, bool isStart = false); // 無敵時間を手動で設定
    void SetTargetPosition(float x, float y); // 移動目的地をセットする
    void ResetInputFlag();                   // 操作済みフラグを解除
    void ResetForTutorial();                 // チュートリアル用に座標と状態を戻す
};
#endif