/**
 * @file GameScene.cpp
 * @brief ゲームプレイシーンの実装
 */

#include "GameScene.h"
#include "Config.h"
#include "AudioManager.h"
#include "DxLib.h"

 // コンストラクタ
GameScene::GameScene()
    : map(MapConfig::TILE_X, MapConfig::TILE_Y, MapConfig::TILE_SIZE)
    , camera(MapConfig::WIDTH, MapConfig::HEIGHT)
    , pauseReason(PauseReason::None)
{
    gameOverTimer = 0;
    isGameOverStarted = false;
    fadeAlpha = 0;
    circleRadius = 600.0f;
}

// シーン初期化
void GameScene::Init()
{
    AudioManager::PlayBGM(BGMType::Game);

    pauseReason = PauseReason::StartWait; // 開始待機状態からスタート

    // 各マネージャーの初期化
    player.Init();
    enemyManager.Init();
    itemManager.Init();
    roundManager.Init();
    scoreManager.Init();

    StartRound();
}

// メイン更新
void GameScene::Update()
{
    // タイトル戻り
    if (CheckDownKey(KEY_INPUT_T))
    {
        nextScene = SceneType::Title;
        return;
    }
    if (CheckDownKey(KEY_INPUT_G)){
        
        nextScene = SceneType::HowToPlay;
    }
    // カットイン演出の同期
    if (enemyManager.IsCutInPlaying())
        pauseReason = PauseReason::CutIn;
    else if (pauseReason == PauseReason::CutIn)
        pauseReason = PauseReason::None;

    // 一時停止状態の処理
    if (pauseReason != PauseReason::None)
    {
        // WASD入力でゲーム開始
        if (pauseReason == PauseReason::StartWait)
        {
            if (CheckDownKey(KEY_INPUT_SPACE))
            {
                pauseReason = PauseReason::None;
            }
        }

        // カットイン中も敵の演出は継続
        if (pauseReason == PauseReason::CutIn)
            UpdateEnemies();

        return;
    }

    // 通常のゲーム進行
    if (UpdateGameOver()) return;

    itemManager.Update(player);
    UpdatePlayer();
    UpdateEnemies();

    UpdateRoundClearBonus();
    UpdateCombat();
    UpdateCamera();
    UpdateRoundFlow();
}

// プレイヤー更新
void GameScene::UpdatePlayer()
{
    player.Update(
        static_cast<int>(camera.GetX()),
        static_cast<int>(camera.GetY())
    );
}

// 敵群の更新と撃破数カウント
void GameScene::UpdateEnemies()
{
    int before = enemyManager.GetEnemyCount();

    enemyManager.Update(
        player.GetX(),
        player.GetY(),
        player.GetRadius(),
        player,
        scoreManager
    );

    int after = enemyManager.GetEnemyCount();

    // 減少した敵の数だけラウンドマネージャーに通知
    int killed = before - after;
    for (int i = 0; i < killed; i++)
    {
        roundManager.OnEnemyKilled();
    }
}

// ラウンドクリア時のスコア加算
void GameScene::UpdateRoundClearBonus()
{
    if (roundManager.GetState() != RoundManager::State::Battle) return;
    if (roundManager.IsRoundFinished()) return;
    if (enemyManager.GetEnemyCount() != 0) return;

    scoreManager.AddEventScore(ScoreEvent::RoundClear, roundManager.GetRound());

    // 残りの敵カウントを強制消化
    while (!roundManager.IsRoundFinished())
    {
        roundManager.OnEnemyKilled();
    }
}

// ラウンドの切り替えフロー
void GameScene::UpdateRoundFlow()
{
    roundManager.Update();

    int enemyNum = roundManager.CalculateEnemyCount();
    
    if (!roundManager.IsRoundFinished()) return;

    roundManager.StartNextRound();
    enemyManager.Clear();

    // 次のラウンドの敵を生成
    enemySpawner.SpawnRoundEnemies(enemyNum, enemyManager);
    roundManager.StartRound(enemyNum);

    player.SetInvincible(120, true); // ラウンド開始時は2秒の無敵
}

// 戦闘関連の更新
void GameScene::UpdateCombat()
{
    laserSystem.Update(player, enemyManager);
    CheckCollision();
}

// 衝突判定
void GameScene::CheckCollision()
{
    const auto& enemies = enemyManager.GetEnemies();

    for (const auto& enemy : enemies)
    {
        if (enemy.IsDying() || enemy.IsDead()) continue;

        // プレイヤーと敵の接触判定
        if (IsCircleCollision(
            player.GetX(), player.GetY(), player.GetRadius(),
            enemy.GetX(), enemy.GetY(), enemy.GetRadius()))
        {
            player.OnDamage();

            scoreManager.ResetMultiplier();
        }
    }
}

// カメラ追従
void GameScene::UpdateCamera()
{
    camera.Update(
        static_cast<int>(player.GetX()),
        static_cast<int>(player.GetY())
    );
}

// ラウンド開始セットアップ
void GameScene::StartRound()
{
    enemyManager.Clear();

    int enemyNum = roundManager.CalculateEnemyCount();

    enemySpawner.SpawnRoundEnemies(enemyNum, enemyManager); 
    roundManager.StartRound(enemyNum); 

    player.SetInvincible(120, true);
}

// ゲーム終了判定
bool GameScene::UpdateGameOver()
{
    if (!player.IsDead()) return false;

    if (!isGameOverStarted)
    {
        isGameOverStarted = true;
        gameOverTimer = 120; // 約2秒

        // BGM切り替え（ここでやると自然）
        AudioManager::PlayBGM(BGMType::GameOver);
    }

    gameOverTimer--;

    if (gameOverTimer < 90) { 
        fadeAlpha += 3;
        if (fadeAlpha > 255) fadeAlpha = 255;
    }
    // 円を縮める
    if (gameOverTimer < 90) {
        circleRadius -= 10.0f;
        if (circleRadius < 0) circleRadius = 0;
    }
    if (gameOverTimer > 0) return true;
    
    scoreManager.UpdateRankings();
    nextScene = SceneType::GameOver;
    return true;
}

// 描画
void GameScene::Draw()
{
    // カットイン時は専用描画
    if (pauseReason == PauseReason::CutIn)
    {
        enemyManager.DrawCutIn();
        return;
    }

    // 地形描画
    map.Draw(camera.GetX(), camera.GetY());
    map.DrawFrame(camera.GetX(), camera.GetY());

    // オブジェクト描画
    enemyManager.Draw(camera.GetX(), camera.GetY());
    laserSystem.Draw(player, camera.GetX(), camera.GetY());
    player.Draw(camera.GetX(), camera.GetY());
    itemManager.Draw(camera.GetX(), camera.GetY());

    // UI描画
    if (pauseReason == PauseReason::StartWait)
    {
        DrawString(500, 300, "スペース：スタート", GetColor(255, 255, 255));
    }

    scoreManager.Draw();

    // デバッグ表示（左上）
    DrawFormatString(20, 60, GetColor(255, 255, 0),
        "Round: %d  Enemy: %d",
        roundManager.GetRound(),
        enemyManager.GetEnemyCount());

    DrawString(20, Screen::HEIGHT - 40, "[T] : タイトルにもどる", GetColor(255, 255, 255));
    enemyManager.DrawScorePopups(camera.GetX(), camera.GetY());

    // プレイヤー死亡時のフェードアウト演出
    if (fadeAlpha > 0)
    {
        int px = static_cast<int>(player.GetX() - camera.GetX());
        int py = static_cast<int>(player.GetY() - camera.GetY());
        int r = static_cast<int>(circleRadius + 0.5f);
        // 全体を暗く
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeAlpha);
        DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(0, 0, 0), TRUE);

        // 円の外側だけ黒くする
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // 上
        DrawBox(0, 0, Screen::WIDTH, py - r, GetColor(0, 0, 0), TRUE);
        // 下
        DrawBox(0, py + r, Screen::WIDTH, Screen::HEIGHT, GetColor(0, 0, 0), TRUE);
        // 左
        DrawBox(0, py - r, px - r, py + r, GetColor(0, 0, 0), TRUE);
        // 右
        DrawBox(px + r, py - r, Screen::WIDTH, py + r, GetColor(0, 0, 0), TRUE);
    }
}