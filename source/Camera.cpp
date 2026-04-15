/**
 * @file Camera.cpp
 * @brief Cameraクラスの実装
 */

#include "Camera.h"
#include "Config.h"

 // コンストラクタ
Camera::Camera(int mapW, int mapH)
    : x(0)
    , y(0)
    , mapWidth(mapW)
    , mapHeight(mapH)
{
}

//  画面の端がマップの境界を超えないように制限をかける 
void Camera::Clamp()
{
    // 左端・上端の制限
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    // 右端の制限（マップ幅 - 画面幅）
    if (x + Screen::WIDTH > mapWidth)
        x = mapWidth - Screen::WIDTH;

    // 下端の制限（マップ高 - 画面高）
    if (y + Screen::HEIGHT > mapHeight)
        y = mapHeight - Screen::HEIGHT;
}

// 指定座標を画面中央に配置
void Camera::SetCenter(int targetX, int targetY)
{
    // ターゲット座標から画面半分のサイズを引いて左上座標を算出
    x = targetX - Screen::WIDTH / 2;
    y = targetY - Screen::HEIGHT / 2;

    Clamp();
}

// プレイヤーへの追従更新
void Camera::Update(int targetX, int targetY)
{
    // 本来あるべきカメラの目標左上位置
    int targetCamX = targetX - Screen::WIDTH / 2;
    int targetCamY = targetY - Screen::HEIGHT / 2;

    // 現在位置から目標位置へ向かって一定割合で接近
    // 数値を小さくするほど、よりゆったりとした追従になる
    x += (targetCamX - x) / 10;
    y += (targetCamY - y) / 10;

    Clamp();
}