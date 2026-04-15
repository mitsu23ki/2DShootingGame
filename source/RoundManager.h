/**
 * @file RoundManager.h
 * @brief ラウンド進行管理の定義
 */

#ifndef ROUND_MANAGER_H_
#define ROUND_MANAGER_H_

class RoundManager {
public:
    // 進行状態
    enum class State {
        Waiting,    // 開始待機
        Battle,     // 戦闘中
        Upgrade     // 強化画面
    };

private:
    // ラウンド情報
    int currentRound;   // 現在のラウンド数
    int enemyCount;     // 出現した敵の総数
    int enemiesKilled;  // 撃破済みの敵数

    // 内部状態
    State state;        // 現在の進行ステータス

public:
    RoundManager();

    // メインループ
    void Init();
    void Update();

    // ラウンド制御
    void StartRound(int enemyNum);    // 指定した敵数でラウンド開始
    void OnEnemyKilled();             // 敵撃破時のカウントアップ
    void StartNextRound();            // ラウンド数を進める

    // 判定
    bool IsRoundFinished() const;     // 全ての敵を倒したか
    bool IsUpgradeTime() const;       // 強化イベントを発生させるか

    // Getter
    int GetRound() const { return currentRound; }
    State GetState() const { return state; }

    // 計算
    int CalculateEnemyCount() const;  // ラウンドに応じた敵数の算出
};

#endif // ROUND_MANAGER_H_