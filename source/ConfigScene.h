/**
 * @file ConfigScene.h
 * @brief 設定画面（音量調整など）のシーン定義
 */

#ifndef CONFIGSCENE_H_
#define CONFIGSCENE_H_

#include "Scene.h"
#include "DxLib.h"
#include "CheckKey.h"
#include "AudioManager.h"
#include "Config.h"

class ConfigScene : public Scene {
private:
    // 状態管理
    SceneType nextScene = SceneType::None; // 次に遷移するシーン
    int select = 0;                        // 現在の選択項目 (0:BGM 1:SE 2:戻る)

public:
    void Init() override {}

    // 更新処理
    void Update() override {

        // 項目移動（W/Sキーでループ選択）
        if (CheckDownKey(KEY_INPUT_W)||(CheckDownKey(KEY_INPUT_UP))) {
            select = (select + 2) % 3;
            AudioManager::PlaySE(SEType::Cursor);
        }
        if (CheckDownKey(KEY_INPUT_S)|| (CheckDownKey(KEY_INPUT_DOWN))) {
            select = (select + 1) % 3;
            AudioManager::PlaySE(SEType::Cursor);
        }

        // 現在の音量取得
        int bgm = AudioManager::GetBGMVolume();
        int se = AudioManager::GetSEVolume();

        // 左右キーによる音量調整
        if (select == 0) {
            if (CheckDownKey(KEY_INPUT_A) || (CheckHitKey(KEY_INPUT_LEFT)))  bgm -= Sound::VOLUME_STEP;
            if (CheckDownKey(KEY_INPUT_D) || (CheckHitKey(KEY_INPUT_RIGHT))) bgm += Sound::VOLUME_STEP;
        }
        if (select == 1) {
            if (CheckDownKey(KEY_INPUT_A) || (CheckHitKey(KEY_INPUT_LEFT)))  se -= Sound::VOLUME_STEP;
            if (CheckDownKey(KEY_INPUT_D) || (CheckHitKey(KEY_INPUT_RIGHT))) se += Sound::VOLUME_STEP;
        }

        // 音量の範囲制限（クランプ処理）
        if (bgm < Sound::VOLUME_MIN) bgm = Sound::VOLUME_MIN;
        if (bgm > Sound::VOLUME_MAX) bgm = Sound::VOLUME_MAX;
        if (se < Sound::VOLUME_MIN) se = Sound::VOLUME_MIN;
        if (se > Sound::VOLUME_MAX) se = Sound::VOLUME_MAX;

        // AudioManagerへの反映
        AudioManager::SetBGMVolume(bgm);
        AudioManager::SetSEVolume(se);

        // シーン遷移（「もどる」選択中にスペースキー）
        if (CheckDownKey(KEY_INPUT_SPACE) && select == 2) {
            nextScene = SceneType::Menu;
            AudioManager::PlaySE(SEType::Select);
        }
    }

    // 描画処理
    void Draw() override {
        // 背景
        DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(100, 200, 100), TRUE);

        int white = GetColor(255, 255, 255);

        // タイトル
        DrawString(520, 120, "サウンド", white);

        // 項目
        DrawItem(420, 220, "BGM", AudioManager::GetBGMVolume(), 0);
        DrawItem(420, 280, "SE", AudioManager::GetSEVolume(), 1);
        DrawItem(420, 340, "もどる", 0, 2);

        // 操作説明
        int guideX = 850;
        int guideY = 580;
        int guideColor = GetColor(255, 255, 255);

        DrawShadowString(guideX, guideY, "W / S : 選択", guideColor);
        DrawShadowString(guideX, guideY + 30, "A / D : 調整", guideColor);
        DrawShadowString(guideX, guideY + 60, "SPACE : 戻る", guideColor);
    }

    // 次シーン取得
    SceneType GetNextScene() const override { return nextScene; }

private:

    void DrawShadowString(int x, int y, const char* str, int color) {
        int black = GetColor(0, 0, 0);
        DrawString(x + 1, y + 1, str, black); // 影
        DrawString(x, y, str, color);         // 本体
    }

    // 項目描画用補助関数
    void DrawItem(int x, int y, const char* name, int value, int index) {
        // 選択中の項目は白、それ以外は灰色で表示
        unsigned int color = (select == index) ? GetColor(255, 255, 255) : GetColor(150, 150, 150);

        if (index == 2) {
            // 「もどる」ボタン
            DrawFormatString(x, y, color, "＞ %s", name);
        }
        else {
            // 音量設定項目
            DrawFormatString(x, y, color, "＞ %s : %d", name, value);
        }
    }
};

#endif // CONFIGSCENE_H_