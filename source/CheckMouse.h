/**
 * @file CheckMouse.h
 * @brief マウスのクリック・離した瞬間の判定管理
 */

#ifndef CHECK_MOUSE_H_
#define CHECK_MOUSE_H_

 /**
  * @brief マウスボタンが押された瞬間を判定
  * @param mouseButton DxLibのマウス入力定数 (例: MOUSE_INPUT_LEFT)
  * @return 押された瞬間のみ 1、それ以外は 0
  */
int CheckDownMouse(int mouseButton);

/**
 * @brief マウスボタンが離された瞬間を判定
 * @param mouseButton DxLibのマウス入力定数 (例: MOUSE_INPUT_LEFT)
 * @return 離された瞬間のみ 1、それ以外は 0
 */
int CheckUpMouse(int mouseButton);

#endif // CHECK_MOUSE_H_