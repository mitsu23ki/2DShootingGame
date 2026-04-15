/**
 * @file ScoreManager.h
 * @brief スコア加算およびランキング保存の管理クラス定義
 */

#ifndef SCORE_MANAGER_H_
#define SCORE_MANAGER_H_

#include "DxLib.h"
#include "Config.h"
#include <stdio.h>
#include <algorithm>
#include <vector>

 // スコア発生イベントの種類
enum class ScoreEvent {
    KillFake,   // 分身を倒した
    KillReal,   // 本物を倒した
    RoundClear  // ラウンドをクリアした
};

class ScoreManager {
private:
    // メンバ変数
    int currentScore = 0;              // 今回のプレイの現在スコア
    int rankings[5] = { 0 };           // 1位～5位のスコア保持用
    float multiplier = 1.0f;            // スコア倍率（初期1.0）
    const char* SAVE_FILE = "Ranking.dat"; // 保存ファイル名

    // 静的メンバ：シーンを跨いで直近のスコアを保持するコンテナ
    static int& LastPlayScoreContainer() {
        static int s_lastScore = 0;
        return s_lastScore;
    }

public:
    // 初期化処理
    void Init() {
        currentScore = 0;
        multiplier = 1.0f; 
        LoadRankings(); // 過去の記録を読み込み
    }

    void ResetMultiplier() {
        multiplier = 1.0f;
    }

    // イベントに応じたスコア加算
    int AddEventScore(ScoreEvent ev, int round = 1) {
        int baseScore = 0;

        switch (ev) {
        case ScoreEvent::KillFake:   baseScore = Round::PTS_FAKE; break;
        case ScoreEvent::KillReal:   baseScore = Round::PTS_REAL; break;
        case ScoreEvent::RoundClear: baseScore = (Round::PTS_ROUND_BASE * round); break;
        }

        int finalScore = static_cast<int>(baseScore * multiplier + 0.5f);

        currentScore += finalScore;

        if (ev == ScoreEvent::KillFake || ev == ScoreEvent::KillReal) {
            multiplier += 0.1f;

            multiplier = floorf(multiplier * 10.0f) / 10.0f;

            if (multiplier > 3.0f) multiplier = 3.0f;
        }

        return finalScore;
    }

    // ランキングの更新と保存
    void UpdateRankings() {
        // 直近のスコアを保存
        LastPlayScoreContainer() = currentScore;

        LoadRankings(); // ファイルから最新の状態を再ロード

        // 一時ベクタに現在のランキングと今回のスコアをまとめる
        std::vector<int> temp;
        for (int i = 0; i < 5; i++) temp.push_back(rankings[i]);
        temp.push_back(currentScore);

        // 降順（大きい順）に並べ替え
        std::sort(temp.begin(), temp.end(), std::greater<int>());

        // 上位5件をメンバ変数に戻す
        for (int i = 0; i < 5; i++) rankings[i] = temp[i];

        SaveRankings(); // ファイルへ書き出し
    }

    // 各種ゲッター
    int GetScore() const { return currentScore; }
    int GetRanking(int index) const { return (index >= 0 && index < 5) ? rankings[index] : 0; }
    int GetLastPlayScore() const { return LastPlayScoreContainer(); }

    // スコア描画（プレイ中用）
    void Draw() const {
        DrawFormatString(20, 20, GetColor(255, 255, 255), "SCORE: %07d", currentScore);
    }

private:
    // ファイルへのセーブ
    void SaveRankings() {
        FILE* fp;
        fopen_s(&fp, SAVE_FILE, "wb");
        if (fp) {
            fwrite(rankings, sizeof(int), 5, fp);
            fclose(fp);
        }
    }

    // ファイルからのロード
    void LoadRankings() {
        FILE* fp;
        fopen_s(&fp, SAVE_FILE, "rb");
        if (fp) {
            fread(rankings, sizeof(int), 5, fp);
            fclose(fp);
        }
        else {
            // ファイルがない場合は0で初期化
            for (int i = 0; i < 5; i++) rankings[i] = 0;
        }
    }
};

#endif // SCORE_MANAGER_H_