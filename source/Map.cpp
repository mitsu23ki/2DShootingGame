/**
 * @file Map.cpp
 * @brief マップ描画処理の実装
 */

#include "Map.h"
#include "DxLib.h"
#include "Config.h"

 // コンストラクタ
Map::Map(int tileX, int tileY, int tile)
    : mapTileX(tileX)
    , mapTileY(tileY)
    , tileSize(tile)
{
    GenerateTileColors(); // 初回のみ色を生成
}

// タイル色生成
void Map::GenerateTileColors()
{
    tileColors.resize(mapTileY);

    for (int y = 0; y < mapTileY; y++)
    {
        tileColors[y].resize(mapTileX);

        for (int x = 0; x < mapTileX; x++)
        {
            // 単調にならないようノイズを加える
            int noise = (x * 13 + y * 7) % 10;

            tileColors[y][x] = GetColor(
                120 + noise,
                90 + noise / 2,
                55
            );
        }
    }
}

// マップ地面描画
void Map::Draw(int cameraX, int cameraY) const
{
    for (int ty = 0; ty < mapTileY; ++ty)
    {
        for (int tx = 0; tx < mapTileX; ++tx)
        {
            // カメラ座標を考慮した描画位置の計算
            int drawX = tx * tileSize - cameraX;
            int drawY = ty * tileSize - cameraY;

            // 画面外のタイルは描画スキップ（クリッピング）
            if (drawX < -tileSize || drawX > Screen::WIDTH ||
                drawY < -tileSize || drawY > Screen::HEIGHT)
            {
                continue;
            }

            DrawBox(
                drawX,
                drawY,
                drawX + tileSize,
                drawY + tileSize,
                tileColors[ty][tx],
                TRUE
            );
        }
    }
}

// マップ外枠描画
void Map::DrawFrame(int cameraX, int cameraY) const
{
    // 全体範囲の計算
    int left = -cameraX;
    int top = -cameraY;
    int right = GetPixelWidth() - cameraX;
    int bottom = GetPixelHeight() - cameraY;

    int t = MapConfig::TILE_SIZE; // 壁の厚み

    unsigned int outerColor = GetColor(110, 70, 40); // 外壁の色
    unsigned int innerColor = GetColor(200, 160, 120); // 装飾線の色

    // 4辺の壁を描画
    DrawBox(left, top, right, top + t, outerColor, TRUE);
    DrawBox(left, bottom - t, right, bottom, outerColor, TRUE);
    DrawBox(left, top, left + t, bottom, outerColor, TRUE);
    DrawBox(right - t, top, right, bottom, outerColor, TRUE);

    // 壁の内側に装飾線を描画
    int inset = 4;
    DrawBox(left + inset, top + inset, right - inset, top + t - inset, innerColor, FALSE);
    DrawBox(left + inset, bottom - t + inset, right - inset, bottom - inset, innerColor, FALSE);
    DrawBox(left + inset, top + inset, left + t - inset, bottom - inset, innerColor, FALSE);
    DrawBox(right - t + inset, top + inset, right - inset, bottom - inset, innerColor, FALSE);
}