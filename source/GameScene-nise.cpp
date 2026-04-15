#include "GameScene.h"
#include "Config.h"
#include"DxLib.h"
#include <cmath>

// コンストラクタ
GameScene::GameScene()
    : map(
        MapConfig::TILE_X,
        MapConfig::TILE_Y,
        MapConfig::TILE_SIZE
    )
    , camera(
        MapConfig::WIDTH,
        MapConfig::HEIGHT
    )
{
}

// 初期化
void GameScene::Init()
{
    // プレイヤー初期化
    player.Init();

    // 敵マネージャ初期化
    enemyManager.Init();

    // ラウンドシステム初期化
    roundManager.Init();

    // 最初のラウンド開始
    StartRound();
}

// 更新処理
void GameScene::Update()
{
    // プレイヤー更新
    player.Update(
        static_cast<int>(camera.GetX()),
        static_cast<int>(camera.GetY())
    );

    if (player.IsDead())
    {
        // とりあえずゲーム終了
        DxLib_End();
        return;
    }

    // 敵更新
    enemyManager.Update(
        player.GetX(),
        player.GetY(),
        player.GetRadius(),
        player
    );

    // 攻撃システム更新
    laserSystem.Update(player, enemyManager);

    // プレイヤーと敵の衝突判定
    CheckCollision();

    // カメラ更新（プレイヤー追従）
    camera.Update(
        static_cast<int>(player.GetX()),
        static_cast<int>(player.GetY())
    );

    // ラウンド更新
    roundManager.Update();

    // ラウンド終了チェック
    if (roundManager.IsRoundFinished())
    {
        // アップグレード時間でなければ次ラウンド開始
        if (!roundManager.IsUpgradeTime())
        {
            StartRound();
        }
    }
}

// 描画処理
void GameScene::Draw()
{
    // マップ描画
    map.Draw(
        static_cast<int>(camera.GetX()),
        static_cast<int>(camera.GetY())
    );

    // マップ枠描画
    map.DrawFrame(
        static_cast<int>(camera.GetX()),
        static_cast<int>(camera.GetY())
    );

    // 敵描画
    enemyManager.Draw(
        static_cast<int>(camera.GetX()),
        static_cast<int>(camera.GetY())
    );

    // レーザー描画
    laserSystem.Draw(
        player,
        static_cast<int>(camera.GetX()),
        static_cast<int>(camera.GetY())
    );

    // プレイヤー描画
    player.Draw(
        static_cast<int>(camera.GetX()),
        static_cast<int>(camera.GetY())
    );
}

// ラウンド開始
void GameScene::StartRound()
{
    // 敵リストをクリア
    enemyManager.Clear();

    // 敵スポーン
    enemySpawner.SpawnRoundEnemies(
        roundManager.GetRound(),
        enemyManager
    );

    // ラウンド開始
    roundManager.StartRound(
        enemyManager.GetEnemyCount()
    );
}

// プレイヤーと敵の衝突判定
void GameScene::CheckCollision()
{
    const auto& enemies = enemyManager.GetEnemies();

    for (const auto& enemy : enemies)
    {
        // 煙（死亡演出中）は当たり判定しない
        if (enemy.IsDying() || enemy.IsDead())
            continue;

        if (IsCircleCollision(
            player.GetX(), player.GetY(), player.GetRadius(),
            enemy.GetX(), enemy.GetY(), enemy.GetRadius()))
        {
            player.OnDamage();
        }
    }
}