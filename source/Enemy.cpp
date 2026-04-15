/**
 * @file Enemy.cpp
 * @brief エネミークラスの実装
 * @details 敵（狸）の移動、AI思考、射撃、被弾・死亡演出などの管理を行う
 */

#include "Enemy.h"
#include "DxLib.h"
#include "Config.h"
#include "EnemyParam.h"
#include "ShotPattern.h"
#include <cmath>
#include <cstdlib>

 // コンストラクタ
Enemy::Enemy(float startX, float startY, EnemyType t)
    : x(startX)
    , y(startY)
    , dirX(0.0f)
    , dirY(0.0f)
    , type(t)
    , speed(gEnemyParam[type].speed)
    , hp(gEnemyParam[type].maxHp)
    , isDead(false)
    , isDying(false)
    , directionChangeTimer(0)
    , radius(0.0f)
    , animCounter(0)
    , isDamaged(false)
    , damageTimer(0)
    , hpBarTimer(0)
    , deathTimer(0)
    , imgIdle(-1)
    , imgIdleDamage(-1)
    , imgIdleReal(-1)
    , imgSmoke(-1)
    , shootTimer(rand() % EnemyConst::TIME_MAX)
    , burstCount(0)
    , burstMax(gEnemyParam[type].burstMax)
    , burstInterval(gEnemyParam[type].burstInterval)
    , restInterval(gEnemyParam[type].restInterval)
    , hasFiredThisRun(false)
    , canBeTargeted(true)
    , canShoot(true)
    , isTutorialStop(false)
    , scoreAdded (false)
{
    imgWalk[0] = imgWalk[1] = -1;
    imgWalkDamage[0] = imgWalkDamage[1] = -1;
    imgWalkReal[0] = imgWalkReal[1] = -1;

    // 種類に応じた初期移動の設定
    if (type == ENEMY_REAL_TANUKI)
        SetRandomOffscreenTarget();
    else
        ChangeDirection();
}

// デストラクタ
Enemy::~Enemy() {}

// 通常画像の設定
void Enemy::SetImages(int idle, int walk[2])
{
    imgIdle = idle;
    imgWalk[0] = walk[0];
    imgWalk[1] = walk[1];

    // 画像サイズを取得し、当たり判定の半径を算出
    GetGraphSize(imgIdle, &width, &height);
    radius = width * EnemyConst::HIT_RADIUS_SCALE;
}

// ダメージ画像の設定
void Enemy::SetDamageImages(int idle, int walk[2])
{
    imgIdleDamage = idle;
    imgWalkDamage[0] = walk[0];
    imgWalkDamage[1] = walk[1];
}

// 本物狸用の画像設定
void Enemy::SetRealImages(int idle, int walk[2])
{
    imgIdleReal = idle;
    imgWalkReal[0] = walk[0];
    imgWalkReal[1] = walk[1];

    // 本物用のサイズと当たり判定を更新
    GetGraphSize(imgIdleReal, &width, &height);
    radius = width * EnemyConst::HIT_RADIUS_SCALE;
}

// 消滅エフェクト画像の設定
void Enemy::SetSmokeImage(int img)
{
    imgSmoke = img;
}

// 更新メイン
void Enemy::Update(
    const std::vector<Enemy>& allEnemies,
    std::vector<Bullet>& bullets,
    float playerX,
    float playerY)
{
    if (UpdateDeath()) return; // 死亡演出中なら以降をスキップ

    UpdateDamage();
    UpdateHPBar();
    UpdateAI(allEnemies, bullets, playerX, playerY);
    Move();

    animCounter++;
}

// 被弾演出の更新
void Enemy::UpdateDamage()
{
    if (!isDamaged) return;

    if (--damageTimer <= 0)
        isDamaged = false;
}

// HPバー表示タイマーの更新
void Enemy::UpdateHPBar()
{
    if (hpBarTimer > 0)
        hpBarTimer--;
}

// 死亡演出の更新
bool Enemy::UpdateDeath()
{
    if (!isDying) return false;

    if (--deathTimer <= 0)
        isDead = true;

    return true;
}

// AI思考
void Enemy::UpdateAI(
    const std::vector<Enemy>& allEnemies,
    std::vector<Bullet>& bullets,
    float playerX,
    float playerY)
{
    if (isTutorialStop) return;

    if (type == ENEMY_FAKE_TANUKI)
    {
        AvoidOthers(allEnemies); // 敵同士の重なり回避

        if (canShoot)
        {
            if (--shootTimer <= 0)
            {
                ShotPattern::FireAimed(bullets, x, y, playerX, playerY, 4.0f);
                shootTimer = EnemyConst::TIME_MIN + rand() % EnemyConst::TIME_MAX;
            }
        }
    }
    else if (type == ENEMY_REAL_TANUKI)
    {
        canBeTargeted = true;
    }
}

// 描画
void Enemy::Draw(int cameraX, int cameraY) const
{
    const int screenX = static_cast<int>(x) - cameraX;
    const int screenY = static_cast<int>(y) - cameraY;
    const int drawX = screenX - width / 2;
    const int drawY = screenY - height / 2;

    // 死亡演出（煙とフェードアウト）
    if (isDying)
    {
        int alpha = deathTimer * 255 / EnemyConst::DEATH_TIME;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawGraph(drawX, drawY, imgSmoke, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        return;
    }

    // アニメーション制御
    const int index = (animCounter / EnemyConst::ANIM_FPS) % 2;
    int graph = -1;

    if (type == ENEMY_REAL_TANUKI)
    {
        graph = (isDamaged && imgWalkDamage[index] != -1)
            ? imgWalkDamage[index]
            : (dirX == 0 && dirY == 0 ? imgIdleReal : imgWalkReal[index]);
    }
    else
    {
        graph = (isDamaged && imgWalkDamage[index] != -1)
            ? imgWalkDamage[index]
            : imgWalk[index];
    }

    if (graph != -1)
    {
        if (dirX < 0) DrawTurnGraph(drawX, drawY, graph, TRUE);
        else DrawGraph(drawX, drawY, graph, TRUE);
    }

    // HPバー描画
    if (hpBarTimer > 0)
    {
        const float hpRate = static_cast<float>(hp) / gEnemyParam[type].maxHp;
        const int barX = screenX - EnemyConst::HP_BAR_WIDTH / 2;
        const int barY = screenY - height / 2 - EnemyConst::HP_BAR_OFFSET_Y;

        // 背景（赤）
        DrawBox(barX, barY, barX + EnemyConst::HP_BAR_WIDTH, barY + EnemyConst::HP_BAR_HEIGHT, GetColor(255, 0, 0), TRUE);
        // 残量（緑）
        DrawBox(barX, barY, barX + static_cast<int>(EnemyConst::HP_BAR_WIDTH * hpRate), barY + EnemyConst::HP_BAR_HEIGHT, GetColor(0, 255, 0), TRUE);
    }
}

// 被ダメージ処理
void Enemy::Damage(int amount)
{
    if (isDead || isDying) return;

    hp -= amount;

    if (hp <= 0)
    {
        hp = 0;
        isDying = true;
        // 本物は演出を長めにとる
        deathTimer = (type == ENEMY_REAL_TANUKI) ? 180 : EnemyConst::DEATH_TIME;
    }

    isDamaged = true;
    damageTimer = EnemyConst::DAMAGE_TIME;
    hpBarTimer = EnemyConst::HP_BAR_TIME;
}

void Enemy::SetRandomOffscreenTarget()
{
    // 出現する辺を決定 (0:左, 1:右, 2:上, 3:下)
    int side = rand() % EnemyConst::SPAWN_SIDE_COUNT;
    float startX, startY;

    switch (side)
    {
    case 0: // 左外から出現
        startX = World::LEFT - EnemyConst::OFFSCREEN_MARGIN;
        startY = static_cast<float>(rand() % (World::BOTTOM - World::TOP) + World::TOP);
        break;

    case 1: // 右外から出現
        startX = World::RIGHT + EnemyConst::OFFSCREEN_MARGIN;
        startY = static_cast<float>(rand() % (World::BOTTOM - World::TOP) + World::TOP);
        break;

    case 2: // 上外から出現
        startX = static_cast<float>(rand() % (World::RIGHT - World::LEFT) + World::LEFT);
        startY = World::TOP - EnemyConst::OFFSCREEN_MARGIN;
        break;

    default: // 下外から出現
        startX = static_cast<float>(rand() % (World::RIGHT - World::LEFT) + World::LEFT);
        startY = World::BOTTOM + EnemyConst::OFFSCREEN_MARGIN;
        break;
    }

    x = startX;
    y = startY;

    // 画面中央付近の目標地点をランダムに決定
    const float margin = 200.0f;
    float targetX = static_cast<float>(
        rand() % static_cast<int>(World::RIGHT - World::LEFT - margin * 2)
        + World::LEFT + margin);

    float targetY = static_cast<float>(
        rand() % static_cast<int>(World::BOTTOM - World::TOP - margin * 2)
        + World::TOP + margin);

    // 目標地点への単位ベクトル（方向）を算出
    float dx = targetX - x;
    float dy = targetY - y;
    float len = std::hypot(dx, dy);

    if (len > 0)
    {
        dirX = dx / len;
        dirY = dy / len;
    }
    else
    {
        dirX = dirY = 0;
    }

    // 次に方向を変える（再出現する）までのタイマーを設定
    directionChangeTimer =
        EnemyConst::TIME_MIN +
        rand() % (EnemyConst::TIME_MAX - EnemyConst::TIME_MIN + 1);
}

// 移動計算
void Enemy::Move()
{
    // 本物狸：画面外からの往復移動
    if (type == ENEMY_REAL_TANUKI)
    {
        x += dirX * speed;
        y += dirY * speed;

        if (x < World::LEFT - EnemyConst::OFFSCREEN_MARGIN ||
            x > World::RIGHT + EnemyConst::OFFSCREEN_MARGIN ||
            y < World::TOP - EnemyConst::OFFSCREEN_MARGIN ||
            y > World::BOTTOM + EnemyConst::OFFSCREEN_MARGIN)
        {
            hasFiredThisRun = false;
            SetRandomOffscreenTarget();
        }
        return;
    }

    // 偽物狸：ランダム徘徊
    if (--directionChangeTimer <= 0)
        ChangeDirection();

    x += dirX * speed;
    y += dirY * speed;

    // ワールド境界での跳ね返り
    const float halfW = width * 0.5f;
    const float halfH = height * 0.5f;

    if (x - halfW < World::LEFT) { x = World::LEFT + halfW; dirX = -dirX; }
    if (x + halfW > World::RIGHT) { x = World::RIGHT - halfW; dirX = -dirX; }
    if (y - halfH < World::TOP) { y = World::TOP + halfH; dirY = -dirY; }
    if (y + halfH > World::BOTTOM) { y = World::BOTTOM - halfH; dirY = -dirY; }
}

// 重なり回避
void Enemy::AvoidOthers(const std::vector<Enemy>& allEnemies)
{
    const float AVOID_RADIUS = radius * 2.0f;
    const float AVOID_FORCE = 0.5f;

    for (const auto& other : allEnemies)
    {
        if (&other == this) continue;

        float dx = x - other.x;
        float dy = y - other.y;
        float distSq = dx * dx + dy * dy;

        if (distSq < AVOID_RADIUS * AVOID_RADIUS)
        {
            float dist = std::sqrt(distSq);
            if (dist > 0.01f)
            {
                dirX += (dx / dist) * AVOID_FORCE;
                dirY += (dy / dist) * AVOID_FORCE;
            }
        }
    }

    // 速度一定のための正規化
    float len = std::sqrt(dirX * dirX + dirY * dirY);
    if (len > 0)
    {
        dirX /= len;
        dirY /= len;
    }
}

// 徘徊方向の変更
void Enemy::ChangeDirection()
{
    float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * DX_PI_F;
    dirX = cosf(angle);
    dirY = sinf(angle);

    directionChangeTimer =
        EnemyConst::TIME_MIN +
        rand() % (EnemyConst::TIME_MAX - EnemyConst::TIME_MIN + 1);
}

void Enemy::ForceBurn(int burnImg)
{
    if (!isDying && !isDead)
    {
        isDying = true;
        deathTimer = 180;
        imgSmoke = burnImg;
    }
}

// 外部設定・状態取得
void Enemy::SetTutorialStop(bool flag) { isTutorialStop = flag; }
void Enemy::SetCanShoot(bool flag) { canShoot = flag; }
bool Enemy::CanBeTargeted() const { return !isDying && !isDead && canBeTargeted; }