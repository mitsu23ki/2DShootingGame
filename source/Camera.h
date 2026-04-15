/**
 * @file Camera.h
 * @brief 2Dゲーム用カメラ制御クラス
 * @details プレイヤーの追従、およびマップ範囲外を表示しないための境界制限を行う
 */

#ifndef CAMERA_H_
#define CAMERA_H_

class Camera
{
private:
    // カメラの左上座標（ワールド座標系）
    int x;
    int y;

    // 制限対象となるマップの全体サイズ
    int mapWidth;
    int mapHeight;

    
    // カメラがマップの範囲外を映さないように座標を補正する
    void Clamp();

public:
    // コンストラクタ
    Camera(int mapWidth, int mapHeight);

   // カメラを指定座標が中心にくるように即座に移動させる
    void SetCenter(int targetX, int targetY);

    // 指定した座標に向かってカメラを滑らかに追従させる
    void Update(int targetX, int targetY);

    // 描画時に差し引くためのカメラ座標取得
    int GetX() const { return x; }
    int GetY() const { return y; }
};

#endif  // CAMERA_H_