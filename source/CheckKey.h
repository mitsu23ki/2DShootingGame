/**
 * @file CheckKey.h
 * @brief キー入力の状態検知
 */

#ifndef CHECK_KEY_H_
#define CHECK_KEY_H_

 /**
  * @brief 指定されたキーが押された瞬間を判定
  * @return 押された瞬間のみ1、それ以外は0
  */
int CheckDownKey(int KeyCode);

/**
 * @brief 指定されたキーが離された瞬間を判定
 * @return 離された瞬間のみ1、それ以外は0
 */
int CheckUpKey(int KeyCode);

#endif // CHECK_KEY_H_