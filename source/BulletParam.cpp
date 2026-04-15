/**
 * @file BulletParam.cpp
 * @brief 弾パラメータの実体定義
 */

#include "BulletParam.h"
#include "DxLib.h"

 /**
  * @brief 各弾タイプの性能設定
  * @details
  * - SMALL: 標準的な黄色い弾
  * - BIG: 避けにくい赤い大きな弾　(BOSに使ってた削除)
  * - FAST: 弾速重視の青い鋭い弾   (削除)
  */
BulletParam gBulletParam[BULLET_MAX] =
{
    // [BULLET_SMALL] 
    { 6.0f,  GetColor(255, 200, 0) },

    // [BULLET_BIG]
    { 12.0f, GetColor(255, 80, 80) },

    // [BULLET_FAST]
    { 8.0f,  GetColor(80, 200, 255) }
};