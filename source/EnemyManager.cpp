/**
 * @file EnemyManager.cpp
 * @brief 敵管理クラスの実装
 * @details 敵と敵弾のライフサイクル、衝突判定、ボスイベントの進行を制御する
 */

#include "EnemyManager.h"
#include "DxLib.h"
#include "ScoreManager.h"
#include <algorithm>
#include "AudioManager.h"

 // コンストラクタ
EnemyManager::EnemyManager()
    : imgIdle(-1)
    , imgIdleDamage(-1)
    , imgSmoke(-1)
    , imgIdleReal(-1)
    , imgCutIn(-1)
    , imgBurning(-1)
    , realTanukiDeadTrigger(false)
    , isCutInPlaying(false)
    , hasPlayedCutIn(false)
    , cutInTimer(0)
{
    imgWalk[0] = imgWalk[1] = -1;
    imgWalkDamage[0] = imgWalkDamage[1] = -1;
    imgWalkReal[0] = imgWalkReal[1] = -1;
}

// デストラクタ
EnemyManager::~EnemyManager()
{
    Clear();

    // グラフィックリソースの解放
    if (imgIdle != -1) DeleteGraph(imgIdle);
    if (imgIdleDamage != -1) DeleteGraph(imgIdleDamage);
    if (imgSmoke != -1) DeleteGraph(imgSmoke);
    if (imgCutIn != -1) DeleteGraph(imgCutIn);
    if (imgBurning != -1) DeleteGraph(imgBurning);
    if (imgIdleReal != -1) DeleteGraph(imgIdleReal);

    for (int i = 0; i < 2; i++)
    {
        if (imgWalk[i] != -1) DeleteGraph(imgWalk[i]);
        if (imgWalkDamage[i] != -1) DeleteGraph(imgWalkDamage[i]);
        if (imgWalkReal[i] != -1) DeleteGraph(imgWalkReal[i]);
    }
}

// 初期化
void EnemyManager::Init()
{
    imgIdle = LoadGraph("Data/fake_0.png");
    imgWalk[0] = LoadGraph("Data/fake_walk1.png");
    imgWalk[1] = LoadGraph("Data/fake_walk2.png");

    imgIdleReal = LoadGraph("Data/enemy_0.png");
    imgWalkReal[0] = LoadGraph("Data/enemy_walk1.png");
    imgWalkReal[1] = LoadGraph("Data/enemy_walk2.png");

    imgIdleDamage = LoadGraph("Data/fake_hit_idle.png");
    imgWalkDamage[0] = LoadGraph("Data/fake_hit_walk1.png");
    imgWalkDamage[1] = LoadGraph("Data/fake_hit_walk2.png");

    imgSmoke = LoadGraph("Data/smoke.png");
    imgCutIn = LoadGraph("Data/cutin.png");
    imgBurning = LoadGraph("Data/enemy_die.png");
}

// 更新メイン
void EnemyManager::Update(
    float playerX,
    float playerY,
    float playerRadius,
    Player& player,
    ScoreManager& scoreManager)
{
    
    CheckBossEvent(); 
    if (isCutInPlaying) 
    {
        UpdateEnemies(playerX, playerY);
        return;
    }
    
    UpdateEnemies(playerX, playerY);

    AddScore(scoreManager);

    UpdateBullets();
    CheckBulletCollision(player);
  
    UpdateScorePopups();
    RemoveDeadObjects();

}

// ボスイベント更新
void EnemyManager::CheckBossEvent()
{

    // すでにカットイン中ならタイマー更新のみ
    if (isCutInPlaying) {
        if (cutInTimer > 0) {
            cutInTimer--;
            if (cutInTimer == 0) {
                isCutInPlaying = false;
                
            }
        }
        return;
    }

    bool bossDying = false;

    for (const auto& enemy : enemies)
    {
        if (enemy.GetType() == ENEMY_REAL_TANUKI && enemy.IsDying())
        {
            bossDying = true;
            break;
        }
    }

    // カットイン演出開始
    if (bossDying && !hasPlayedCutIn)
    {
        isCutInPlaying = true;
        cutInTimer = 120; // 2秒間
        hasPlayedCutIn = true;
        AudioManager::PlaySE(SEType::Decide);
        // 全ての偽物を消去
        for (auto& enemy : enemies)
        {
            if (enemy.GetType() == ENEMY_FAKE_TANUKI)
                enemy.ForceBurn(imgBurning);
        }

        bullets.clear(); // 画面内の弾を一掃
    }

    // カウントダウン
    if (cutInTimer > 0)
    {
        cutInTimer--;
        if (cutInTimer == 0)
        {
            isCutInPlaying = false;
        }
    }
}

// 敵の更新
void EnemyManager::UpdateEnemies(float playerX, float playerY)
{
    for (auto& enemy : enemies)
        enemy.Update(enemies, bullets, playerX, playerY);

}

// 弾の更新
void EnemyManager::UpdateBullets()
{
    for (auto& b : bullets)
        b.Update();
}

// 敵弾とプレイヤーの当たり判定
void EnemyManager::CheckBulletCollision(Player& player)
{
    const float px = player.GetX();
    const float py = player.GetY();
    const float pr = player.GetRadius();

    for (auto& b : bullets)
    {
        float dx = b.GetX() - px;
        float dy = b.GetY() - py;
        float distSq = dx * dx + dy * dy;
        float hit = pr + b.GetRadius();

        if (distSq <= hit * hit)
        {
            player.OnDamage();
            b.Kill();
        }
    }
}

// スコア加算
void EnemyManager::AddScore(ScoreManager& scoreManager)
{
    for (auto& enemy : enemies)
    {
        if (!enemy.IsDying()) continue;
        if (enemy.IsScoreAdded()) continue;

        int addScore = 0;

        if (enemy.GetType() == ENEMY_REAL_TANUKI)
        {
            addScore = scoreManager.AddEventScore(ScoreEvent::KillReal);
        }
        else
        {
            addScore = scoreManager.AddEventScore(ScoreEvent::KillFake);
        }

        enemy.SetScoreAdded(true);

        scorePopups.push_back({
            enemy.GetX(),
            enemy.GetY(),
            addScore,
            60
            });
    }
}

void EnemyManager::UpdateScorePopups()
{
    for (auto& p : scorePopups)
    {
        p.y -= 1.0f;   
        p.timer--;
    }

    scorePopups.erase(
        std::remove_if(scorePopups.begin(), scorePopups.end(),
            [](const ScorePopup& p) { return p.timer <= 0; }),
        scorePopups.end());
}

void EnemyManager::DrawScorePopups(int cameraX, int cameraY) const
{
    for (const auto& p : scorePopups)
    {
        int x = static_cast<int>(p.x) - cameraX;
        int y = static_cast<int>(p.y) - cameraY;

        DrawFormatString(x, y, GetColor(255, 255, 0), "+%d", p.score);
    }
}

// 死んだオブジェクトのメモリ削除
void EnemyManager::RemoveDeadObjects()
{
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return e.IsDead(); }),
        enemies.end());

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.IsDead(); }),
        bullets.end());
}

// 描画
void EnemyManager::Draw(int cameraX, int cameraY) const
{
    for (const auto& enemy : enemies) enemy.Draw(cameraX, cameraY);
    for (const auto& b : bullets) b.Draw(cameraX, cameraY);
}

// カットインの描画
void EnemyManager::DrawCutIn() const
{
    if (isCutInPlaying)
        DrawGraph(0, 0, imgCutIn, TRUE);
}

// 敵の新規生成
void EnemyManager::SpawnEnemy(float x, float y, EnemyType type)
{
    enemies.emplace_back(x, y, type);
    Enemy& enemy = enemies.back();
    enemyCounted.push_back(false);
    if (type == ENEMY_REAL_TANUKI)
    {
        int walk[2] = { imgWalkReal[0], imgWalkReal[1] };
        enemy.SetRealImages(imgIdleReal, walk);
        enemy.SetSmokeImage(imgBurning);
    }
    // 画像設定
    else
    {
        enemy.SetImages(imgIdle, imgWalk);
        enemy.SetDamageImages(imgIdleDamage, imgWalkDamage);
        enemy.SetSmokeImage(imgSmoke);
    }
}

// チュートリアル等での停止した偽物生成
void EnemyManager::SpawnOneFake(float x, float y)
{
    SpawnEnemy(x, y, ENEMY_FAKE_TANUKI);

    if (!enemies.empty())
    {
        enemies.back().SetSpeed(0.0f);
    }
}

// 全クリア
void EnemyManager::Clear()
{
    enemies.clear();
    bullets.clear();
    enemyCounted.clear();
    isCutInPlaying = false;
    cutInTimer = 0;
    realTanukiDeadTrigger = false;
    hasPlayedCutIn = false;
}

// ゲッター群
int EnemyManager::GetEnemyCount() const { return static_cast<int>(enemies.size()); }

const std::vector<Enemy>& EnemyManager::GetEnemies() const { return enemies; }
const std::vector<Bullet>& EnemyManager::GetBullets() const { return bullets; }
bool EnemyManager::IsCutInPlaying() const { return isCutInPlaying; }

// ボス撃破フラグの確認とリセット
bool EnemyManager::IsRealTanukiDefeated()
{
    if (realTanukiDeadTrigger)
    {
        realTanukiDeadTrigger = false;
        return true;
    }
    return false;
}

std::vector<Enemy>& EnemyManager::GetEnemies()
{
    return enemies;
}