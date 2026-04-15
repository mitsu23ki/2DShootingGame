#ifndef HOW_TO_PLAY_SCENE_H_
#define HOW_TO_PLAY_SCENE_H_

#include "Scene.h"
#include "DxLib.h"
#include "AudioManager.h"
#include "CheckKey.h"
#include "Config.h"

class HowToPlayScene : public Scene {
private:
    SceneType nextScene = SceneType::None;
    int imgGuide;

public:
    HowToPlayScene() : imgGuide(-1) {}

    void Init() override {
        // 画像の読み込み（パスは実際のフォルダに合わせてください）
        imgGuide = LoadGraph("Data/guide.png");    // キーボード　サイズ５１２(
     
       
    }

    void Update() override {
        // 選択（戻る）はスペースキー
        if (CheckDownKey(KEY_INPUT_SPACE)) {
            AudioManager::PlaySE(SEType::Select);
            nextScene = SceneType::Menu;
        }
    }

    void Draw() override {


        DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(100, 200, 100), TRUE);
        
        DrawGraph(0, 0, imgGuide, TRUE);
       
        int yellow = GetColor(255, 255, 0);

        // 戻る案内だけ重ねる
        DrawFormatString(880, 640, yellow, "SPACE：戻る");
    }

    SceneType GetNextScene() const override { return nextScene; }

    ~HowToPlayScene() {
        DeleteGraph(imgGuide);
    }
};

#endif