#ifndef MENUSCENE_H_
#define MENUSCENE_H_

#include "Scene.h"
#include "DxLib.h"
#include "CheckKey.h"
#include "Config.h"

// 列挙型はクラスの外に置く
enum class MenuOrder {
    GameStart,
    Tutorial,
    Ranking,
    Config,
    Max
};

class MenuScene : public Scene {
private:
    SceneType nextScene = SceneType::None;
    MenuOrder currentSelect = MenuOrder::GameStart;

    int imgUsagiGun, imgUsagiBook, imgUsagiTrophy, imgUsagiWrench, imgFukidashi;
    int animCounter;

public:
    MenuScene() : animCounter(0) {
        imgUsagiGun = imgUsagiBook = imgUsagiTrophy = imgUsagiWrench = imgFukidashi = -1;
    }

    // デストラクタ
    virtual ~MenuScene() {
        DeleteGraph(imgUsagiGun);
        DeleteGraph(imgUsagiBook);
        DeleteGraph(imgUsagiTrophy);
        DeleteGraph(imgUsagiWrench);
        DeleteGraph(imgFukidashi);
    }

    void Init() override {
        imgUsagiGun = LoadGraph("Data/player_gun.png");     
        imgUsagiBook = LoadGraph("Data/player_book.png");
        imgUsagiTrophy = LoadGraph("Data/Trophy.png");
        imgUsagiWrench = LoadGraph("Data/player_Wrench.png");
        imgFukidashi = LoadGraph("Data/Image (24).png");
    }

    void Update() override {
        animCounter++;
        // 選択移動
        if (CheckDownKey(KEY_INPUT_S) || CheckDownKey(KEY_INPUT_DOWN)) {
            AudioManager::PlaySE(SEType::Cursor);
            currentSelect = static_cast<MenuOrder>((static_cast<int>(currentSelect) + 1) % static_cast<int>(MenuOrder::Max));
        }
        if (CheckDownKey(KEY_INPUT_W)||CheckDownKey(KEY_INPUT_UP)) {
            AudioManager::PlaySE(SEType::Cursor);
            currentSelect = static_cast<MenuOrder>((static_cast<int>(currentSelect) + static_cast<int>(MenuOrder::Max) - 1) % static_cast<int>(MenuOrder::Max));
        }

        // Tキーでタイトル画面
        if (CheckHitKey(KEY_INPUT_T))
        {
            nextScene = SceneType::Title;
        }

        if (CheckDownKey(KEY_INPUT_SPACE) || CheckDownKey(KEY_INPUT_RETURN)) {
            AudioManager::PlaySE(SEType::Select);
            if (currentSelect == MenuOrder::GameStart) nextScene = SceneType::Game;
            if (currentSelect == MenuOrder::Ranking) nextScene = SceneType::GameOver;
            if (currentSelect == MenuOrder::Tutorial)nextScene = SceneType::Tutorial;
            if (currentSelect == MenuOrder::Config) nextScene = SceneType::Config;
        }
    }

    void Draw() override {
        // 背景緑
        DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(100, 200, 100), TRUE);

        // メニュー項目の描画
        DrawMenuItem(350, 200, "スコアアタック", MenuOrder::GameStart);
        DrawMenuItem(350, 260, "あそびかた", MenuOrder::Tutorial);
        DrawMenuItem(350, 320, "ランキング", MenuOrder::Ranking);
        DrawMenuItem(350, 380, "音量設定", MenuOrder::Config);

        // 選択中のうさぎ（立ち絵）と吹き出しの描画
        DrawCharacterAndMessage();

        int guideY = Screen::HEIGHT - 40;
        unsigned int guideColor = GetColor(255, 255, 255);

        DrawString(51, Screen::HEIGHT - 69, "Tキー：タイトル", GetColor(0, 0, 0));
        DrawString(50, Screen::HEIGHT - 70, "Tキー：タイトル", guideColor);
        DrawString(51, guideY + 1, "選択： [W/S] [UP/DOWN]    決定： [SPACE] [ENTER]", GetColor(0, 0, 0));
        DrawString(50, guideY, "選択： [W/S] [UP/DOWN]    決定： [SPACE] [ENTER]", guideColor);
    }

    SceneType GetNextScene() const override { return nextScene; }

private:
    void DrawMenuItem(int x, int y, const char* text, MenuOrder type) {
        unsigned int color = (currentSelect == type) ? GetColor(255, 255, 255) : GetColor(40, 70, 40);
        float scale = (currentSelect == type) ? 1.2f : 1.0f; // 選択中は少し大きく
        DrawFormatString(x, y, color, "%s %s", (currentSelect == type ? "★" : "  "), text);
    }

    void DrawCharacterAndMessage() {
        // うさぎの描画基準位置
        int usagiX = 780;
        int usagiY = 320;

        // 吹き出しの描画位置
        int fukidashiX = usagiX + 60;
        int fukidashiY = usagiY - 140; // うさぎより上に表示

        int currentImg = -1;
        const char* msg = "";

        // 選択項目に応じた設定
        switch (currentSelect) {
        case MenuOrder::GameStart:
            currentImg = imgUsagiGun;
            msg = "\nたぬきと遊ぶだきゅ"; 
            break;
        case MenuOrder::Tutorial:
            currentImg = imgUsagiBook;
            msg = "\nふむふむだきゅ";
            break;
        case MenuOrder::Ranking:
            currentImg = imgUsagiTrophy;
            msg = "\nみんなの記録だきゅ";
            break;
        case MenuOrder::Config:
            currentImg = imgUsagiWrench;
            msg = "\n音量を調節だぬ";
            break;
        }

      
        // 立ち絵描画
        if (currentImg != -1) {
            DrawGraph(usagiX, usagiY , currentImg, TRUE);
        }

        // 吹き出し描画
        if (imgFukidashi != -1) {
            // 吹き出し本体
            DrawGraph(fukidashiX, fukidashiY, imgFukidashi, TRUE);

            // メッセージ描画
            int textX = fukidashiX + 50; // 吹き出しの左端からの余白
            int textY = fukidashiY + 100; // 吹き出しの上端からの余白

            // 吹き出しの文字
            DrawString(textX, textY , msg, GetColor(0, 0, 0));
        }
    }
}; 

#endif