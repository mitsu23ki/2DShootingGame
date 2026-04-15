/**
 * @file Title.h
 * @brief タイトル画面のシーン定義（タヌキとうさぎの行進演出付き）
 */

#ifndef TITLE_H_
#define TITLE_H_

#include "Scene.h"
#include "DxLib.h"
#include "CheckKey.h"
#include "CheckMouse.h"
#include "Config.h"
#include "AudioManager.h"
#include <vector>

 // タイトル演出用のタヌキデータ
struct FakeTanuki {
    float x, y;    // 位置
    float speed;   // 移動速度
};

class TitleScene : public Scene {
private:
    // 状態管理
    SceneType nextScene = SceneType::None; // 次に遷移するシーン

    // グラフィックハンドル
    int imgUsagi[2];      // うさぎアニメ用
    int imgRealTanuki[2]; // 本物タヌキ（葉っぱあり）
    int imgFakeTanuki[2]; // 偽物タヌキ（葉っぱなし）
    int imgLogo;          // タイトルロゴ

    // 演出用変数
    int animCounter;               // アニメーション用カウンタ
    float realTanukiX;             // 先頭の本物タヌキのX座標
    float usagiX;                  // 最後尾のうさぎのX座標
    std::vector<FakeTanuki> fakes; // 間を歩く偽物タヌキ軍団

public:
    // コンストラクタ
    TitleScene()
        : animCounter(0)
        , realTanukiX(-100.0f)
        , usagiX(-800.0f)
        , imgLogo(-1)
    {
        // ハンドル配列を初期化
        for (int i = 0; i < 2; i++) {
            imgUsagi[i] = -1;
            imgRealTanuki[i] = -1;
            imgFakeTanuki[i] = -1;
        }
    }

    // デストラクタ
    ~TitleScene() {
        // 全画像の解放処理
        for (int i = 0; i < 2; i++) {
            DeleteGraph(imgUsagi[i]);
            DeleteGraph(imgRealTanuki[i]);
            DeleteGraph(imgFakeTanuki[i]);
        }
        DeleteGraph(imgLogo);
    }

    // 初期化処理
    void Init() override {
        // 画像リソースのロード
        imgUsagi[0] = LoadGraph("Data/player_walk1.png");
        imgUsagi[1] = LoadGraph("Data/player_walk2.png");
        imgRealTanuki[0] = LoadGraph("Data/enemy_hit_walk1.png");
        imgRealTanuki[1] = LoadGraph("Data/enemy_hit_walk2.png");
        imgFakeTanuki[0] = LoadGraph("Data/fake_hit_walk1.png");
        imgFakeTanuki[1] = LoadGraph("Data/fake_hit_walk2.png");
        imgLogo = LoadGraph("Data/TitleLogo.png");

        AudioManager::PlayBGM(BGMType::Title);

        // 行列の配置設定
        const float SPACING = 100.0f; // キャラクター間の間隔
        realTanukiX = -100.0f;        // 本物（先頭）の初期位置

        // 偽物軍団を生成して後ろに並べる
        fakes.clear();
        for (int i = 0; i < 10; i++) {
            FakeTanuki f;
            f.x = realTanukiX - (SPACING * (i + 1));
            f.y = 620.0f;
            f.speed = 3.0f;
            fakes.push_back(f);
        }

        // うさぎを軍団の最後尾に配置
        usagiX = realTanukiX - (SPACING * 12);
    }

    // 更新処理
    void Update() override {
        animCounter++;
        float commonSpeed = 3.0f; // 全員共通の移動速度

        // 全員を一斉に右へ移動させる
        realTanukiX += commonSpeed;
        for (auto& f : fakes) {
            f.x += commonSpeed;
        }
        usagiX += commonSpeed;

        // 全員が画面右外に消えたら行列をリセット
        if (usagiX > Screen::WIDTH + 100) {
            Init();
        }

        // スペースキーまたは左クリックでメニューへ
        if (CheckDownKey(KEY_INPUT_SPACE) || (CheckDownMouse(MOUSE_INPUT_LEFT)))
        {
            AudioManager::PlaySE(SEType::Select);
            nextScene = SceneType::Menu;
        }
    }

    // 描画処理
    void Draw() override {
        // アニメーションコマの決定
        int animIdx = (animCounter / PlayerConst::ANIM_FPS) % 2;

        // 背景（芝生）
        DrawBox(0, 0, Screen::WIDTH, Screen::HEIGHT, GetColor(100, 200, 100), TRUE);

        // 行列の描画（後ろのキャラから順に描画）
        // 1. うさぎ
        DrawGraph(static_cast<int>(usagiX), 600, imgUsagi[animIdx], TRUE);

        // 2. 偽物タヌキ軍団
        for (const auto& f : fakes) {
            DrawGraph(static_cast<int>(f.x), static_cast<int>(f.y), imgFakeTanuki[animIdx], TRUE);
        }

        // 3. 本物タヌキ（先頭）
        DrawGraph(static_cast<int>(realTanukiX), 620, imgRealTanuki[animIdx], TRUE);

        // タイトルロゴの描画（中央でふわふわ上下させる）
        if (imgLogo != -1) {
            int centerX = Screen::WIDTH / 2;
            int centerY = 250;
            float bounceY = sinf(animCounter * 0.05f) * 15.0f;
            DrawRotaGraph(centerX, centerY + (int)bounceY, 1.0f, 0.0f, imgLogo, TRUE);
        }

        // スタート案内の点滅表示
        if ((animCounter / 40) % 2 == 0) {
            DrawString(320, 520, "PRESS SPACE TO START", GetColor(255, 255, 255));
        }

        // 終了キーの案内
        DrawString(320, 550, "CLOSE GAME : [ESC]", GetColor(255, 255, 255));
    }

    // 次シーン取得
    SceneType GetNextScene() const override { return nextScene; }
};

#endif // TITLE_H_