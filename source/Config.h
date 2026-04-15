/**
 * @file Config.h
 * @brief ゲーム全体で使用する定数・バランスパラメータの管理
 * @details 画面解像度、キャラクター性能、マップサイズなどの魔法数字を一括管理する
 */

#ifndef CONFIG_H_
#define CONFIG_H_

 /**
  * @namespace Screen
  * @brief 画面表示に関する設定
  */
namespace Screen
{
    // 16:9のアスペクト比を維持し、汎用性の高いHD解像度を採用
    constexpr int WIDTH = 1280;
    constexpr int HEIGHT = 720;
}

/**
 * @namespace PlayerConst
 * @brief プレイヤー（うさぎ）の基本性能
 */
namespace PlayerConst
{
    // --- 初期位置 ---
    constexpr int START_X = 400;
    constexpr int START_Y = 300;

    // --- 画像・当たり判定サイズ ---
    constexpr int WIDTH = 128;
    constexpr int HEIGHT = 128;

    // NOTE: 見た目よりも一回り小さく設定し、プレイヤーが「理不尽」に感じない判定にしている
    constexpr int HIT_WIDTH = 60;
    constexpr int HIT_HEIGHT = 110;

    // --- ステータス ---
    constexpr float SPEED = 5.0f;
    constexpr int   MAX_HP = 3;

    // --- 演出関連 ---
    constexpr int ANIM_FPS = 10;
    constexpr int INVINCIBLE_TIME = 60; // 被弾後の無敵時間（1秒間 / 60fps想定）
    constexpr int BLINK_INTERVAL = 5;  // 点滅周期。視認性を考慮し5F以上に設定
}

/**
 * @namespace Laser
 * @brief プレイヤーのメイン武器
 */
namespace Laser
{
    // TODO: 強化アイテム取得時にRANGEを伸ばす処理をPlayerクラスに検討
    constexpr float RANGE = 250.0f;
    constexpr int   DAMAGE = 1;
    constexpr int   WIDTH = 6;

    // 多段ヒット防止用の待機時間。EnemyのDAMAGE_TIMEと同期
    constexpr int DAMAGE_INTERVAL = 30;
}

/**
 * @namespace EnemyConst
 * @brief 敵キャラ（タヌキ）の共通設定
 */
namespace EnemyConst
{
    // --- サイズ・判定挙動 ---
    constexpr float DEFAULT_RADIUS = 20.0f;
    constexpr float HIT_RADIUS_SCALE = 0.3f; // FIXME: 画像の中心点ズレに対応するオフセットを将来的に追加

    // 出現までの待機フレーム範囲
    constexpr int TIME_MIN = 60;
    constexpr int TIME_MAX = 180;

    // --- 演出 ---
    constexpr int ANIM_FPS = 20;
    constexpr int DAMAGE_TIME = Laser::DAMAGE_INTERVAL;
    constexpr int DEATH_TIME = 60;
    constexpr int FADE_START = 30; // 消滅アニメーションの中盤(30F)からフェード開始

    // --- HPバー表示 ---
    constexpr int HP_BAR_WIDTH = 40;
    constexpr int HP_BAR_HEIGHT = 6;
    constexpr int HP_BAR_OFFSET_Y = 10;  // キャラ頭上への表示オフセット
    constexpr int HP_BAR_TIME = 180; // 非ダメージ時の表示持続時間

    // --- スポーン制御 ---
    constexpr float OFFSCREEN_MARGIN = 50.0f; // 画面外からの急な出現を抑えるマージン
    constexpr int   MAX_ALPHA = 255;
    constexpr int   SPAWN_SIDE_COUNT = 4;     // 本物狸の出現候補（上下左右）
}

/**
 * @namespace ItemConst
 * @brief アイテムの出現と効果
 */
namespace ItemConst
{
    constexpr int SPAWN_INTERVAL = 600; // 10秒に1回
    constexpr int POWER_INVINCIBLE_TIME = 300; // 5秒間の無敵
}

/**
 * @namespace Round
 * @brief ゲームの進行・スコア設計
 */
namespace Round
{
    // 難易度曲線：急激な上昇を避けるため、乗算ベースでスケーリング
    constexpr int BASE_ENEMY = 1;
    constexpr int ROUND_SCALE = 2;
    constexpr int UPGRADE_INTERVAL = 3;

    // スコア配分
    constexpr int PTS_FAKE = 1000;
    constexpr int PTS_REAL = 5000;
    constexpr int PTS_ROUND_BASE = 10000;
    constexpr int RANKING_COUNT = 5;
}

/**
 * @namespace MapConfig
 * @brief マップタイルの配置設定
 */
namespace MapConfig {
    // DxLibの描画最適化を考慮し、16の倍数(48px)を採用
    constexpr int TILE_SIZE = 48;
    constexpr int TILE_X = 35;
    constexpr int TILE_Y = 35;
    constexpr int FRAME_TILE = 1; // 侵入不可な壁タイルの厚み

    // -- マップ全体サイズ（ピクセル） -- 
    constexpr int WIDTH = TILE_X * TILE_SIZE;
    constexpr int HEIGHT = TILE_Y * TILE_SIZE;
}

/**
 * @namespace World
 * @brief キャラクターの移動可能範囲
 */
namespace World
{
    // マップ外周の壁にめり込まないための境界線
    constexpr int LEFT = MapConfig::FRAME_TILE * MapConfig::TILE_SIZE;
    constexpr int TOP = MapConfig::FRAME_TILE * MapConfig::TILE_SIZE;
    constexpr int RIGHT = MapConfig::WIDTH - (MapConfig::FRAME_TILE * MapConfig::TILE_SIZE);
    constexpr int BOTTOM = MapConfig::HEIGHT - (MapConfig::FRAME_TILE * MapConfig::TILE_SIZE);
}

/**
 * @namespace Sound
 * @brief 音響設定
 */
namespace Sound
{
    constexpr int VOLUME_MAX = 255;
    constexpr int VOLUME_MIN = 0;
    constexpr int VOLUME_DEFAULT = 180;
    constexpr int VOLUME_STEP = 5;
}

/**
 * @namespace UI
 * @brief ユーザーインターフェースの装飾
 */
namespace UI
{
    // ライフ（ハート）表示
    constexpr int HEART_RADIUS = 8;
    constexpr int HEART_SPACING = 40;
    constexpr int HEART_MARGIN_BOTTOM = 40;

    // HACK: 将来的に AssetManager 等でカラーパレットとして一括管理を検討
    constexpr int HEART_COLOR_R = 255;
    constexpr int HEART_COLOR_G = 80;
    constexpr int HEART_COLOR_B = 120;

    constexpr int HEART_EMPTY_R = 255;
    constexpr int HEART_EMPTY_G = 255;
    constexpr int HEART_EMPTY_B = 255;
}

#endif // CONFIG_H_