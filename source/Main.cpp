/**
 * @file Main.cpp
 * @brief エントリポイントおよびメインループの実装
 * @author M.Mitsuki
 * @date 2026/01/22 新規作成
 * * Copyright (c) 2026 M.Mitsuki
 */

#include "DxLib.h"
#include "GameScene.h"
#include "Scene.h"
#include "Title.h"
#include "Config.h"
#include "MenuScene.h"
#include "GameOverScene.h"
#include "AudioManager.h"
#include "pause.h"
#include "ConfigScene.h"
#include "TutorialScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // ウィンドウモードの設定
    ChangeWindowMode(TRUE);

    // 画面解像度とカラービットの設定
    SetGraphMode(Screen::WIDTH, Screen::HEIGHT, 32);

    // Dxライブラリの初期化
    if (DxLib_Init() == -1)
    {
        return -1; // 初期化失敗時は終了
    }

    // ダブルバッファリング設定
    SetDrawScreen(DX_SCREEN_BACK);

    // オーディオアセットの一括読み込み
    AudioManager::LoadAll();

    // シーン管理
    Scene* currentScene = nullptr;
    SceneType currentType = SceneType::Title; // 開始シーンの設定

    // 初回シーンの生成と初期化
    currentScene = new TitleScene();
    currentScene->Init();

    // メインループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // 画面クリア
        ClearDrawScreen();

        // 現在のシーンの更新と描画
        if (currentScene != nullptr)
        {
            currentScene->Update();
            currentScene->Draw();
        }

        // シーン遷移のチェックと切り替え
        if (currentScene != nullptr)
        {
            SceneType nextType = currentScene->GetNextScene();
            if (nextType != SceneType::None)
            {
                // 旧シーンの破棄
                delete currentScene;
                currentScene = nullptr;

                // 遷移先
                if (nextType == SceneType::Title) {
                    currentScene = new TitleScene();
                }
                else if (nextType == SceneType::Menu) {
                    currentScene = new MenuScene();
                }
                else if (nextType == SceneType::HowToPlay) {
                    currentScene = new HowToPlayScene();
                }
                else if (nextType == SceneType::Tutorial) {
                    currentScene = new TutorialScene();
                }
                else if (nextType == SceneType::Game) {
                    currentScene = new GameScene();
                }
                else if (nextType == SceneType::Config) {
                    currentScene = new ConfigScene();
                }
                else if (nextType == SceneType::GameOver) {
                    currentScene = new GameOverScene();
                }

                // 新シーンの初期化
                if (currentScene != nullptr) {
                    currentScene->Init();
                }
            }
        }

        // 画面反映
        ScreenFlip();
    }

    // 終了処理
    AudioManager::Terminate();
    DxLib_End();

    return 0;
}