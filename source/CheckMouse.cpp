/**
 * @file CheckMouse.cpp
 * @brief マウス入力のトリガー判定の実装
 */

#include "DxLib.h"
#include "CheckMouse.h"

namespace
{
    // マウスボタン最大数
    constexpr int MOUSE_BUTTON_MAX = 8;

    // 前フレームの状態を保持する
    int downBuffer[MOUSE_BUTTON_MAX] = {}; // 押下判定用
    int upBuffer[MOUSE_BUTTON_MAX] = {};   // 解放判定用

    /**
     * @brief DxLibのマウス定数を配列のインデックスに変換
     */
    int GetMouseIndex(int button)
    {
        switch (button)
        {
        case MOUSE_INPUT_LEFT:   return 0;
        case MOUSE_INPUT_RIGHT:  return 1;
        case MOUSE_INPUT_MIDDLE: return 2;
        default:                 return 0;
        }
    }

    /**
     * @brief 特定のボタンが現在押されているかを取得
     * @return 1 (押されている) / 0 (離されている)
     */
    static int GetMouseState(int button)
    {
        // GetMouseInput() の戻り値とビット論理積をとり、フラグが立っているか判定
        return (GetMouseInput() & button) ? 1 : 0;
    }
}

/**
 * @brief 押し下げられた瞬間を検出
 */
int CheckDownMouse(int mouseButton)
{
    int result = 0;
    int state = GetMouseState(mouseButton); // 現在の状態取得
    int index = GetMouseIndex(mouseButton); // 配列インデックスへの変換

    // 0(離) -> 1(押) への変化を捉える
    if (downBuffer[index] == 0 && state == 1)
    {
        result = 1;
    }

    downBuffer[index] = state; // 状態保存
    return result;
}

/**
 * @brief 離された瞬間を検出
 */
int CheckUpMouse(int mouseButton)
{
    int result = 0;
    int state = GetMouseState(mouseButton); // 現在の状態取得
    int index = GetMouseIndex(mouseButton); // 配列インデックスへの変換

    // 1(押) -> 0(離) への変化を捉える
    if (upBuffer[index] == 1 && state == 0)
    {
        result = 1;
    }

    upBuffer[index] = state; // 状態保存
    return result;
}