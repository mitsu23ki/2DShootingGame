/**
 * @file Player.cpp
 * @brief プレイヤークラスの実装
 * @details プレイヤー（うさぎ）の移動計算、描画演出、当たり判定、
 * および体力や無敵時間などのステータス更新処理を記述する。
 */

#include "Player.h"
#include "DxLib.h"
#include "Config.h"
#include "CheckMouse.h"
#include <cmath>
#include "AudioManager.h"

// コンストラクタ
Player::Player() {
    // スポーン座標に画像サイズの半分を足して、中心座標を算出
    float startX = PlayerConst::START_X + PlayerConst::WIDTH * 0.5f;
    float startY = PlayerConst::START_Y + PlayerConst::HEIGHT * 0.5f;

    playerX = startX;
    playerY = startY;
    targetX = startX;
    targetY = startY;

    speed = static_cast<float>(PlayerConst::SPEED);
    isMoving = false;
    isFlip = false;

    imgIdle = -1;
    imgWalk[0] = -1;
    imgWalk[1] = -1;
    animCounter = 0;

    imgDead = -1;
    isDeadPlayed = false;

    radius = 0.0f;
    hp = PlayerConst::MAX_HP;

    invincibleTimer = 0;
    powerInvincibleTimer = 0;
    isStartInvincible = false;

    hasMovedOnce = false;
}

// デストラクタ
Player::~Player() {

    if (imgIdle != -1) DeleteGraph(imgIdle);
    if (imgDead != -1) DeleteGraph(imgDead);
    for (int i = 0; i < 2; i++) {
        if (imgWalk[i] != -1) DeleteGraph(imgWalk[i]);
    }
}

// 初期化
void Player::Init() {
    // 矩形サイズから平均的な半径を算出（円判定用）
    radius = (PlayerConst::HIT_WIDTH + PlayerConst::HIT_HEIGHT) * 0.25f;

    // 二重ロード防止
    if (imgIdle != -1) return;

    imgIdle = LoadGraph("Data/player.png");
    imgWalk[0] = LoadGraph("Data/player_walk1.png");
    imgWalk[1] = LoadGraph("Data/player_walk2.png");
    imgDead = LoadGraph("Data/player_die.png");
}

// 更新
void Player::Update(int cameraX, int cameraY) {
    HandleInput(cameraX, cameraY);
    Move();
    CheckWorldBounds();

    UpdateAnimation();
    UpdateInvincible();
}

// 入力処理
void Player::HandleInput(int cameraX, int cameraY) {
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);

    if (CheckDownMouse(MOUSE_INPUT_LEFT)) {
        // スクリーン座標をワールド座標に変換して目的地を設定
        float worldX = static_cast<float>(mouseX + cameraX);
        float worldY = static_cast<float>(mouseY + cameraY);

        SetTargetPosition(worldX, worldY);
        hasMovedOnce = true;
    }
}

// 移動処理
void Player::SetTargetPosition(float x, float y) {
    targetX = x;
    targetY = y;
    isMoving = true;
}

void Player::Move() {
    if (!isMoving) return;

    float dx = targetX - playerX;
    float dy = targetY - playerY;
    float length = std::hypot(dx, dy); // 目的地までの距離

    // 進行方向に基づいた反転フラグの更新
    if (dx < 0) isFlip = true;
    else if (dx > 0) isFlip = false;

    // 目的地に十分近い場合は座標を固定して停止
    if (length <= speed) {
        playerX = targetX;
        playerY = targetY;
        isMoving = false;
        animCounter = 0;
        return;
    }

    // ベクトルの正規化を行い、一定速度で移動
    if (length > 0.0f) {
        playerX += (dx / length) * speed;
        playerY += (dy / length) * speed;
    }
}

// マップ外への移動制限
void Player::CheckWorldBounds() {
    float halfW = PlayerConst::HIT_WIDTH * 0.5f;
    float halfH = PlayerConst::HIT_HEIGHT * 0.5f;
    bool hit = false;

    // 各境界での押し戻し処理
    if (playerX - halfW < World::LEFT) {
        playerX = World::LEFT + halfW;
        hit = true;
    }
    if (playerX + halfW > World::RIGHT) {
        playerX = World::RIGHT - halfW;
        hit = true;
    }
    if (playerY - halfH < World::TOP) {
        playerY = World::TOP + halfH;
        hit = true;
    }
    if (playerY + halfH > World::BOTTOM) {
        playerY = World::BOTTOM - halfH;
        hit = true;
    }

    // 壁に当たったら移動を強制中断
    if (hit) {
        isMoving = false;
        animCounter = 0;
    }
}

// アニメーション更新
void Player::UpdateAnimation() {
    if (isMoving) animCounter++;
}

// 無敵タイマー更新
void Player::UpdateInvincible() {
    if (invincibleTimer > 0) {
        invincibleTimer--;
        // タイマー終了時に演出フラグもリセット
        if (invincibleTimer <= 0) isStartInvincible = false;
    }

    if (powerInvincibleTimer > 0) powerInvincibleTimer--;
}

// 描画メイン
void Player::Draw(int cameraX, int cameraY) const {
    DrawPlayer(cameraX, cameraY);
    DrawTargetMarker(cameraX, cameraY);
    DrawHP();
}

// 体力描画
void Player::DrawHP() const {
    const int heartSpacing = UI::HEART_SPACING;
    const int totalWidth = PlayerConst::MAX_HP * heartSpacing;

    // 画面中央下部に配置するための座標計算
    const int startX = (Screen::WIDTH - totalWidth) / 2 + heartSpacing / 2;
    const int drawY = Screen::HEIGHT - UI::HEART_MARGIN_BOTTOM;

    const unsigned int fullColor = GetColor(UI::HEART_COLOR_R, UI::HEART_COLOR_G, UI::HEART_COLOR_B);
    const unsigned int emptyColor = GetColor(UI::HEART_EMPTY_R, UI::HEART_EMPTY_G, UI::HEART_EMPTY_B);

    for (int i = 0; i < PlayerConst::MAX_HP; i++) {
        const int x = startX + i * heartSpacing;
        const unsigned int color = (i < hp) ? fullColor : emptyColor;
        DrawHeart(x, drawY, color);
    }
}

// ハートの図形描画
void Player::DrawHeart(int x, int y, unsigned int color) const {
    const int r = UI::HEART_RADIUS;

    // 図形パーツの比率計算
    const int offset = static_cast<int>(r * 0.8f);
    const int triWidth = static_cast<int>(r * 1.8f);
    const int triHeight = static_cast<int>(r * 2.2f);
    const int triOffsetY = static_cast<int>(r * 0.6f);

    DrawCircle(x - offset, y - r, r, color, TRUE); // 左上
    DrawCircle(x + offset, y - r, r, color, TRUE); // 右上
    DrawCircle(x, y, r, color, TRUE);              // 中央

    // 下側の尖った部分三角形
    DrawTriangle(
        x - triWidth, y - triOffsetY,
        x + triWidth, y - triOffsetY,
        x, y + triHeight,
        color, TRUE);
}

// 無敵状態の判定（各タイマーのいずれかが有効なら無敵）
bool Player::IsInvincible() const {
    return invincibleTimer > 0 || powerInvincibleTimer > 0;
}

// プレイヤー本体の描画
void Player::DrawPlayer(int cameraX, int cameraY) const {
    // 状態に応じた色調変更
    if (powerInvincibleTimer > 0) {
        // パワーアップ中はカラフル演出
        SetDrawBright(GetRand(255), GetRand(255), GetRand(255));
    }
    else if (invincibleTimer > 0 && !isStartInvincible &&
        (invincibleTimer / PlayerConst::BLINK_INTERVAL) % 2 == 0) {
        // ダメージ無敵中は赤く点滅
        SetDrawBright(255, 100, 100);
        if (IsDead()) {
            SetDrawBright(255, 255, 255);
        }
    }
    else {
        SetDrawBright(255, 255, 255);
    }

    int drawX = static_cast<int>(playerX) - cameraX - PlayerConst::WIDTH / 2;
    int drawY = static_cast<int>(playerY) - cameraY - PlayerConst::HEIGHT / 2;

    int graph = imgIdle;

    // 死亡時は倒れた画像
    if (IsDead()) {
        graph = imgDead;
    }
    else if (isMoving) {
        int index = (animCounter / PlayerConst::ANIM_FPS) % 2;
        graph = imgWalk[index];
    }

    if (isFlip) DrawTurnGraph(drawX, drawY, graph, TRUE);
    else DrawGraph(drawX, drawY, graph, TRUE);

    SetDrawBright(255, 255, 255); // 輝度を元に戻す
}

// 移動目的地のマーカー描画
void Player::DrawTargetMarker(int cameraX, int cameraY) const {
    if (!isMoving) return;

    int markerX = static_cast<int>(targetX) - cameraX;
    int markerY = static_cast<int>(targetY) - cameraY;

    DrawCircle(markerX, markerY, 18, GetColor(0, 200, 255), FALSE);
    DrawCircle(markerX, markerY, 6, GetColor(0, 200, 255), TRUE);
}

// 矩形当たり判定の取得
HitRect Player::GetHitRect() const {
    HitRect rect{};
    int halfW = PlayerConst::HIT_WIDTH / 2;
    int halfH = PlayerConst::HIT_HEIGHT / 2;

    rect.left = static_cast<int>(playerX) - halfW;
    rect.right = static_cast<int>(playerX) + halfW;
    rect.top = static_cast<int>(playerY) - halfH;
    rect.bottom = static_cast<int>(playerY) + halfH;

    return rect;
}

float Player::GetRadius() const {
    return radius;
}

// 被ダメージ処理
void Player::OnDamage() {
    if (IsInvincible()) return; // 無敵中なら無視

    hp--;
    if (hp < 0) hp = 0;
    if (hp == 0 && !isDeadPlayed) {
        AudioManager::PlaySE(SEType::PlayerDead);
        isDeadPlayed = true;
        isMoving = false; // 動きを止める
    }
    invincibleTimer = PlayerConst::INVINCIBLE_TIME;
}

// 無敵アイテム
void Player::ActivatePowerInvincible(int time) {
    powerInvincibleTimer = time;
}

// 手動無敵設定
void Player::SetInvincible(int time, bool isStart) {
    invincibleTimer = time;
    isStartInvincible = isStart;
}

// 回復処理
void Player::RecoverHP(int amount) {
    hp += amount;
    if (hp > PlayerConst::MAX_HP) hp = PlayerConst::MAX_HP;
}

bool Player::IsDead() const {
    return hp <= 0;
}

int Player::GetHP() const {
    return hp;
}

// チュートリアル・再試行用のリセット処理
void Player::ResetForTutorial() {
    float startX = PlayerConst::START_X + PlayerConst::WIDTH * 0.5f;
    float startY = PlayerConst::START_Y + PlayerConst::HEIGHT * 0.5f;

    playerX = startX;
    playerY = startY;
    targetX = startX;
    targetY = startY;

    hp = PlayerConst::MAX_HP;
    isMoving = false;
    animCounter = 0;
    isFlip = false;
    hasMovedOnce = false;
    isDeadPlayed = false;
    invincibleTimer = 0;
    powerInvincibleTimer = 0;
    isStartInvincible = false;
}

void Player::ResetInputFlag() {
    hasMovedOnce = false;
}

bool Player::HasMoved() const {
    return hasMovedOnce;
}