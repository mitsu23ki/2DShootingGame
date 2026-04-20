/**
 * @file TutorialScene.h
 * @brief 基本操作を学ぶチュートリアルシーンの定義
 */

#ifndef TUTORIALSCENE_H_
#define TUTORIALSCENE_H_

#include "Scene.h"
#include "Player.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "LaserSystem.h"
#include "ScoreManager.h"
#include "ItemManager.h"
#include "Fade.h"
class TutorialScene : public Scene {
public:
    TutorialScene();
    ~TutorialScene() = default;

    // 基本メソッド
    void Init() override;   // チュートリアルの初期化
    void Update() override; // 各ステップの更新処理
    void Draw() override;   // 描画およびガイド表示
    SceneType GetNextScene() const override; // 次のシーン（Game等）を取得

private:
    //チュートリアル段階の定義
    enum class Step {
        Move,      // 移動の練習
        Attack,    // 攻撃の練習
        Damage,    // ダメージの体験
        ItemHeal,  // 回復アイテムの説明
        ItemPower, // パワーアップアイテムの説明
        End        // 終了案内
    };
    Step step;     // 現在のステップ
    Step nextStep; // フェード後に移行するステップ
    Fade fade;
    // アイテム関連の状態
    enum class ItemPhase { Heal, Power, Wait };
    ItemPhase itemPhase;

    // 進行フラグ
    bool enemyKilled;    // 敵を倒したか
    bool canShoot;       // 射撃許可
    bool isStepComplete; // 現在の課題をクリアしたか
    bool isWaitingNext;  // 次のステップへの待機状態

    // 演出・管理用
    bool isFading;       // フェード演出中か
    int messageTimer;    // メッセージ表示用カウンタ
    int prevHP;          // HP変化検知用
    SceneType nextScene; // 最終的な遷移先シーン

    // ゲーム構成要素
    Player player;
    Camera camera;
    EnemyManager enemyManager;
    LaserSystem laserSystem;
    ScoreManager dummyScore; // チュートリアル用のスコア管理
    ItemManager itemManager;

    // ステップ別更新処理
    void UpdateMove();      // 移動ステップのロジック
    void UpdateAttack();    // 攻撃ステップのロジック
    void UpdateDamage();    // ダメージステップのロジック
    void UpdateItemHeal();  // 回復アイテムステップのロジック
    void UpdateItemPower(); // パワーアップステップのロジック
    void UpdateEnd();       // 終了処理

    // 共通処理
    void ChangeStep(Step next); // ステップ切り替え（フェード開始）
};

#endif // TUTORIALSCENE_H_