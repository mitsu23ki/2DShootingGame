/**
 * @file RoundManager.cpp
 * @brief ラウンド進行管理の実装
 */

#include "RoundManager.h"
#include "Config.h"

 // コンストラクタ
RoundManager::RoundManager()
    : currentRound(1)
    , enemyCount(0)
    , enemiesKilled(0)
    , state(State::Battle)
{
}

// 初期化
void RoundManager::Init()
{
    currentRound = 1;
    enemyCount = 0;
    enemiesKilled = 0;
    state = State::Battle;
}

// ラウンド開始
void RoundManager::StartRound(int enemyNum)
{
    enemyCount = enemyNum;
    enemiesKilled = 0;
    state = State::Battle;
}

// 敵撃破通知
void RoundManager::OnEnemyKilled()
{
    // 二重カウント防止のガード処理
    if (enemiesKilled < enemyCount)
    {
        enemiesKilled++;
    }
}

// 更新処理
void RoundManager::Update()
{
    if (state != State::Battle) return;

    // 進行停止防止
    if (enemyCount <= 0) return;

    // 規定数に達していないなら継続
    if (enemiesKilled < enemyCount) return;

    // 特定ラウンドごとの強化インターバル判定
    if (currentRound % Round::UPGRADE_INTERVAL == 0)
    {
        state = State::Upgrade;
    }
}

// ラウンド終了判定
bool RoundManager::IsRoundFinished() const
{
    // 敵が 0 の場合は終了とみなす
    if (enemyCount <= 0) return true;

    return enemiesKilled >= enemyCount;
}

// 強化タイミング判定
bool RoundManager::IsUpgradeTime() const
{
    return state == State::Upgrade;
}

// 次ラウンド開始
void RoundManager::StartNextRound()
{
    currentRound++;
    enemiesKilled = 0;
    enemyCount = 0;
    state = State::Battle;
}

// ラウンドごとの敵数計算
int RoundManager::CalculateEnemyCount() const
{
    return 2 + currentRound + (currentRound / 3);
}