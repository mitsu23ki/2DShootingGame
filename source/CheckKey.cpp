/**
 * @file CheckKey.cpp
 * @brief キー入力の瞬間検出処理の実装
 */

#include "CheckKey.h"
#include "DxLib.h"

namespace CheckKey
{
    // 各キーの「前回フレームの状態」を保持する
    int downBuffer[256] = { 0 };  // 押下判定用
    int upBuffer[256] = { 0 };    // 離脱判定用
}

/**
 * @brief キーが押された瞬間を判定
 */
int CheckDownKey(int KeyCode)
{
    int result = 0;

    // 現在の入力状態を取得
    int keyState = CheckHitKey(KeyCode);

    // 「前回は 0」かつ「現在は 1」なら押された瞬間
    if (CheckKey::downBuffer[KeyCode] == 0 && keyState == 1)
    {
        result = 1;
    }

    // 次回判定のために状態を保存
    CheckKey::downBuffer[KeyCode] = keyState;

    return result;
}

/**
 * @brief キーが離された瞬間を判定
 */
int CheckUpKey(int KeyCode)
{
    int result = 0;

    // 現在の入力状態を取得
    int keyState = CheckHitKey(KeyCode);

    // 「前回は 1」かつ「現在は 0」なら離された瞬間
    if (CheckKey::upBuffer[KeyCode] == 1 && keyState == 0)
    {
        result = 1;
    }

    // 次回判定のために状態を保存
    CheckKey::upBuffer[KeyCode] = keyState;

    return result;
}