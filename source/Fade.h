/**
* @class Fade
* @brief フェードクラス
*/

#ifndef FADE_H_
#define FADE_H_

class Fade
{
private:
	bool m_fadeFlag;						// フェード中かどうか
	int m_time;								// フェードにかける時間（ミリ秒）
	int m_elapsedTime;						// フェード開始からの経過時間（ミリ秒）
	int m_beginBright;						// 開始輝度
	int m_endBright;						// 終了輝度
	int m_lerpBright;						// 線形補間された輝度値

	int Lerp(int begin, int end, float t);	// 線形補間

public:
	Fade();									// コンストラクタ

	void Update(int deltaTime);				// 更新
	void Draw();			// 描画
	void SetFadeIn(int millisecond);		// フェードイン開始
	void SetFadeOut(int millisecond);		// フェードアウト開始
	bool IsFade() { return m_fadeFlag; };	// フェード中かどうか
	bool IsFinished() const { return !m_fadeFlag; }

};

#endif