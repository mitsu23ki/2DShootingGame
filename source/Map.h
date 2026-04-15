/**
 * @file Map.h
 * @brief マップクラスの定義
 * @details タイルベースの地形描画と外枠の管理を行う
 */

#ifndef MAP_H_
#define MAP_H_

#include <vector>

class Map {
private:
    // マップ規格
    int mapTileX;   // 横方向のタイル数
    int mapTileY;   // 縦方向のタイル数
    int tileSize;   // 1タイルのサイズ

    // 描画キャッシュ
    std::vector<std::vector<unsigned int>> tileColors; // タイルごとの色データ

    // 内部ロジック
    void GenerateTileColors(); // マップ生成時に色をランダム化

public:
    Map(int tileX, int tileY, int tile);

    // 描画
    void Draw(int cameraX, int cameraY) const;      // 地面のタイル描画
    void DrawFrame(int cameraX, int cameraY) const; // 進入不可エリアの外枠描画

    // Getter
    int GetPixelWidth()  const { return mapTileX * tileSize; } // マップ全体の幅
    int GetPixelHeight() const { return mapTileY * tileSize; } // マップ全体の高さ
};

#endif // MAP_H_