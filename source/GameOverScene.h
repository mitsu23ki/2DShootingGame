/**
 * @file GameOverScene.h
 * @brief ゲームオーバー画面のシーン定義
 */

#ifndef GAMEOVERSCENE_H_
#define GAMEOVERSCENE_H_

#include "Scene.h"
#include "DxLib.h"
#include "CheckKey.h"
#include "ScoreManager.h"
#include "Config.h"
#include <cmath>
#include "AudioManager.h"

 // ゲームオーバー時の選択肢
enum class GameOverOrder {
    Retry,       // ゲームをやり直す
    BackToTitle, // タイトル画面へ戻る
    Max
};

class GameOverScene : public Scene {
private:
    // 状態管理
    SceneType nextScene = SceneType::None;             // 次に遷移するシーン
    GameOverOrder currentSelect = GameOverOrder::Retry; // 現在の選択項目
    ScoreManager scoreManager;                         // スコア・ランキング管理

    // 演出・リソース
    int imgForeground; // 前面イラストのハンドル
    int animCounter;   // 選択肢の揺れや点滅用カウンタ

public:
    GameOverScene() : imgForeground(-1), animCounter(0) {}

    // デストラクタ
    virtual ~GameOverScene() {
        if (imgForeground != -1) DeleteGraph(imgForeground); // 読み込んだ画像を解放
    }

    // 初期化処理
    void Init() override {
        scoreManager.Init(); // スコアデータの読み込み
        animCounter = 0;
        AudioManager::PlayBGM(BGMType::GameOver);
        imgForeground = LoadGraph("Data/GameOverIllust.png"); // イラストのロード
    }

    // 更新処理
    void Update() override {
        animCounter++;

        // 上下キー入力による選択項目の切り替え
        if (CheckDownKey(KEY_INPUT_S) || CheckDownKey(KEY_INPUT_W)) {
            AudioManager::PlaySE(SEType::Cursor);
            currentSelect = (currentSelect == GameOverOrder::Retry)
                ? GameOverOrder::BackToTitle : GameOverOrder::Retry;
        }

        // 決定キー（スペース）によるシーン遷移確定
        if (CheckDownKey(KEY_INPUT_SPACE)) {
            AudioManager::PlaySE(SEType::Select);
            nextScene = (currentSelect == GameOverOrder::Retry)
                ? SceneType::Game : SceneType::Title;
        }
    }

    // 描画処理
    void Draw() override {
        // 背景の描画
        DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(100, 200, 100), TRUE);

        // 今回のスコア表示
        int myScore = scoreManager.GetLastPlayScore();
        DrawFormatString(Screen::WIDTH / 2 - 80, 60, GetColor(255, 255, 255), "thank you");
        DrawFormatString(Screen::WIDTH / 2 - 100, 40, GetColor(255, 255, 255), "YOUR SCORE: %07d", myScore);

        // ランキングの表示
        DrawString(Screen::WIDTH / 2 - 100, 120, " TOP 5 RANKING ", GetColor(255, 255, 0));

        bool alreadyHighlighted = false; // 今回のスコアを強調したかどうかの判定用
        for (int i = 0; i < 5; i++) {
            int rankScore = scoreManager.GetRanking(i);
            unsigned int color = GetColor(40, 70, 40); // 通常時の色

            // ランクインした今回のスコアを点滅させる
            if (!alreadyHighlighted && myScore > 0 && rankScore == myScore) {
                color = (animCounter / 30 % 2 == 0) ? GetColor(255, 255, 255) : GetColor(255, 255, 0);
                alreadyHighlighted = true;
            }

            DrawFormatString(Screen::WIDTH / 2 - 80, 150 + (i * 30), color, "%d. %07d PT", i + 1, rankScore);
        }

        // 前面イラストの描画
        if (imgForeground != -1) {
            DrawGraph(0, 0, imgForeground, TRUE);
        }

        // 選択項目の描画
        DrawOption(Screen::WIDTH / 2 - 60, 350, "ゲームスタート", GameOverOrder::Retry);
        DrawOption(Screen::WIDTH / 2 - 60, 400, "タイトルへ", GameOverOrder::BackToTitle);
    }

    // 次のシーンを取得
    SceneType GetNextScene() const override { return nextScene; }

private:
    // 選択肢描画用補助関数
    void DrawOption(int x, int y, const char* text, GameOverOrder type) {
        bool isSelected = (currentSelect == type);
        unsigned int color = isSelected ? GetColor(255, 255, 255) : GetColor(40, 70, 40);

        if (isSelected) {
            // 選択中の演出（カーソルの表示と左右への揺れ）
            float slide = sinf(animCounter * 0.15f) * 5.0f;
            DrawFormatString(x - 30 + (int)slide, y, GetColor(255, 255, 0), "★");

            // 可読性向上のための影
            DrawString(x + 2, y + 2, text, GetColor(0, 0, 0));
        }
        DrawString(x, y, text, color);
    }
};

#endif // GAMEOVERSCENE_H_