/**
 * @file Enemy.h
 * @brief 敵クラスの定義
 * @details 敵（狸）の移動、AI思考、射撃、被弾・死亡演出などを管理する
 */

#ifndef ENEMY_H_
#define ENEMY_H_

#include "EnemyParam.h"
#include <vector>
#include "Bullet.h"

class Enemy {
private:
    // 基本情報
    EnemyType type;

    // 位置・移動
    float x;                      // 現在のX座標（ワールド座標）
    float y;                      // 現在のY座標（ワールド座標）
    float dirX;                   // 移動方向のX成分
    float dirY;                   // 移動方向のY成分
    float speed;                  // 移動スピード
    int directionChangeTimer;     // 次に移動方向を変えるまでの時間
    float targetX;                // 本物の敵が目指す目的地のX座標
    float targetY;                // 本物の敵が目指す目的地のY座標

    // ステータス
    int hp;                       // 現在の体力
    bool isDead;                  // 完全に消滅したかどうか
    bool isDying;                 // 死亡演出中かどうか

    // 当たり判定
    float radius;                 // 円判定に使う当たり判定の半径

    // 描画・アセット
    int width;                    // 画像の横幅
    int height;                   // 画像の縦幅
    int imgIdle;                  // 待機中の画像ハンドル
    int imgWalk[2];               // 歩行アニメーション用の画像ハンドル
    int imgIdleDamage;            // 被弾時の待機画像
    int imgWalkDamage[2];         // 被弾時の歩行アニメーション画像
    int imgIdleReal;              // 本物の敵の待機画像
    int imgWalkReal[2];           // 本物の敵の歩行アニメーション画像
    int imgSmoke;                 // 撃破時の煙エフェクト画像
    int animCounter;              // アニメーション切り替え用のカウンタ

    // ダメージ・演出
    bool isDamaged;               // 今ダメージを受けている最中か
    int damageTimer;              // 被弾演出を表示する残り時間
    int hpBarTimer;               // HPゲージを表示しておく残り時間
    int deathTimer;               // 死亡演出（煙など）の残り時間

    // 射撃
    int shootTimer;               // 次に弾を撃つまでの待機時間
    int burstCount;               // 3連射などの連射中の発射数
    int burstMax;                 // 一度の攻撃で何連射するか
    int burstInterval;            // 連射中の弾と弾の間隔
    int restInterval;             // 次の連射攻撃に移るまでの休憩時間
    bool hasFiredThisRun;         // 現在の攻撃ターンで既に発射したか
    bool canBeTargeted;           // プレイヤーの攻撃対象になるか
    bool canShoot;                // 弾を撃てる状態かどうか
    bool scoreAdded;              // スコア加算が済んでいるか

    // チュートリアル制御
    bool isTutorialStop;          //  チュートリアル中に動きを止めるフラグ

    // 内部処理
    void Move();                        // 移動計算を実行する
    void ChangeDirection();             // 徘徊する方向をランダムに決める
    void SetRandomOffscreenTarget();    // 画面外の目的地をランダムに設定する
    void AvoidOthers(                   // 敵同士が重ならないように押し返す
        const std::vector<Enemy>& allEnemies); 
    void UpdateDamage();          // 被弾状態のタイマー更新とフラグ管理を行う
    void UpdateHPBar();           // HPゲージの表示時間を更新する
    bool UpdateDeath();           // 死亡演出を更新し、終了したらtrueを返す
    void UpdateAI(                // 移動・衝突回避・射撃の思考ルーチンを実行する
        const std::vector<Enemy>& allEnemies,
        std::vector<Bullet>& bullets,
        float playerX,
        float playerY);

public:
    // 生成 / 破棄
    Enemy(float startX, float startY, EnemyType type);
    ~Enemy();

    // 更新
    void Update(
        const std::vector<Enemy>& allEnemies,
        std::vector<Bullet>& bullets,
        float playerX,
        float playerY);
    
    // 描画
    void Draw(int cameraX, int cameraY) const;

    // 初期設定
    void SetImages(int idle, int walk[2]);             // 通常時のアニメーション画像を設定
    void SetDamageImages(int idle, int walk[2]);       // 被弾時の画像を設定
    void SetRealImages(int idle, int walk[2]);         // 本物の敵用の画像を設定
    void SetSmokeImage(int img);                       // 撃破時の煙画像を設定
    void SetSpeed(float s) { speed = s; }              // 移動速度を変更
    void SetTutorialStop(bool flag);                   // チュートリアル用の停止状態を切り替え
    void SetCanShoot(bool flag);                       // 弾を撃つ許可・禁止を切り替え

    // 状態取得
    float GetX() const { return x; }                   // 現在のX座標を取得
    float GetY() const { return y; }                   // 現在のY座標を取得
    float GetRadius() const { return radius; }         // 当たり判定の半径を取得
    EnemyType GetType() const { return type; }         // 敵の種類（本物・偽物）を返す
    bool CanBeTargeted() const;                        // 攻撃対象にできるかチェック
    bool IsDead() const { return isDead; }             // 完全に消滅したかチェック
    bool IsDying() const { return isDying; }           // 死亡演出（消滅中）かチェック
    int GetDeathTimer() const { return deathTimer; }   // 演出終了までの残り時間を取得
    bool IsScoreAdded() const { return scoreAdded; }   // スコア加算済みかチェック
    void SetScoreAdded(bool flag) { scoreAdded = flag; } // スコア加算フラグを設定

    // 状態操作
    void Damage(int amount);                           // 体力を減らし被弾状態にする
    void ForceBurn(int burnImg);                       // 指定の画像で強制的に撃破演出を開始
};

#endif