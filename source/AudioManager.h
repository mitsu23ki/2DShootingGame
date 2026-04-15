/**
 * @file AudioManager.h
 * @brief オーディオ管理クラス（シングルトン）の定義
 */

#ifndef AUDIO_MANAGER_H_
#define AUDIO_MANAGER_H_

#include "DxLib.h"
#include <map>
#include <string>

 // 音源の識別子
enum class BGMType { Title, Game, GameOver, None };
enum class SEType { Select, Decide, Cursor, PlayerDead, None };

class AudioManager {
private:
    // リソース管理
    std::map<BGMType, int> bgmMap;      // BGMのハンドル管理
    std::map<SEType, int> seMap;        // SEのハンドル管理
    BGMType currentBGM = BGMType::None; // 現在再生中のBGM種別

    // 音量設定
    static int bgmVolume; // BGMの共有音量
    static int seVolume;  // SEの共有音量

    // シングルトン実体の取得
    static AudioManager& Instance() {
        static AudioManager instance;
        return instance;
    }

public:
    // 状態確認
    static bool IsPlayingSE(SEType type) {
        auto& am = Instance();
        return CheckSoundMem(am.seMap[type]) == 1; // 再生中なら true
    }

    // セットアップ
    static void LoadAll() {
        auto& am = Instance();

        // BGMファイルの読み込み
        am.bgmMap[BGMType::Title] = LoadSoundMem("Data/MusicTitle.mp3");
        am.bgmMap[BGMType::Game] = LoadSoundMem("Data/MusicGame.mp3");
        am.bgmMap[BGMType::GameOver] = LoadSoundMem("Data/MusicGameOver.mp3");

        // SEファイルの読み込み
        am.seMap[SEType::Select] = LoadSoundMem("Data/SESelect.mp3");
        am.seMap[SEType::Cursor] = LoadSoundMem("Data/SECursor.mp3");
        am.seMap[SEType::Decide] = LoadSoundMem("Data/kirakira.mp3");
        am.seMap[SEType::PlayerDead] = LoadSoundMem("Data/player_dead.mp3");

        // 全体の初期音量を調整
        for (auto& pair : am.bgmMap) ChangeVolumeSoundMem(180, pair.second);
    }

    // BGM制御
    static void PlayBGM(BGMType type) {
        auto& am = Instance();
        if (am.currentBGM == type) return; // 同じ曲なら何もしない

        StopBGM(); // 前の曲を止める

        if (type != BGMType::None) {
            ChangeVolumeSoundMem(bgmVolume, am.bgmMap[type]);
            PlaySoundMem(am.bgmMap[type], DX_PLAYTYPE_LOOP); // ループ再生
        }

        am.currentBGM = type;
    }

    static void StopBGM() {
        auto& am = Instance();
        if (am.currentBGM != BGMType::None) {
            StopSoundMem(am.bgmMap[am.currentBGM]);
        }
        am.currentBGM = BGMType::None;
    }

    // SE制御
    static void PlaySE(SEType type) {
        auto& am = Instance();
        if (type != SEType::None) {
            ChangeVolumeSoundMem(seVolume, am.seMap[type]);
            PlaySoundMem(am.seMap[type], DX_PLAYTYPE_BACK); // バックグラウンド再生
        }
    }

    // クリーンアップ
    static void Terminate() {
        auto& am = Instance();
        for (auto& p : am.bgmMap) DeleteSoundMem(p.second);
        for (auto& p : am.seMap) DeleteSoundMem(p.second);
    }

    // 音量設定
    static void SetBGMVolume(int volume) {
        bgmVolume = volume;

        auto& am = Instance();
        if (am.currentBGM != BGMType::None) {
            ChangeVolumeSoundMem(bgmVolume, am.bgmMap[am.currentBGM]);
        }
    }

    static void SetSEVolume(int volume) {
        seVolume = volume;

        auto& am = Instance();
        for (auto& p : am.seMap) {
            ChangeVolumeSoundMem(seVolume, p.second);
        }
    }

    // 音量取得
    static int GetBGMVolume() { return bgmVolume; }
    static int GetSEVolume() { return seVolume; }
};

#endif // AUDIO_MANAGER_H_