#include "DxLib.h"
#include "Fade.h"

Fade::Fade()
{
	m_fadeFlag = false;
	m_time = 0;
	m_elapsedTime = 0;
	m_beginBright = 0;
	m_endBright = 0;
	m_lerpBright = 0;
}

int Fade::Lerp(int begin, int end, float t)
{
	// 線形補間
	return begin + (end - begin) * t;

}

void Fade::Update(int deltaTime)
{
	// フェード中でなければ処理しない
	if (m_fadeFlag == false)
	{
		return;
	}

	// フェード開始時からの累計経過時間を保持
	m_elapsedTime += deltaTime;
	if (m_elapsedTime < m_time)
	{
		// 累計経過時間から線形補間して0～-255を出す
		m_lerpBright = Lerp(m_beginBright, m_endBright, (float)m_elapsedTime / (float)m_time);
	}
	else
	{
		// 累計経過時間が指定された秒数を超えたらフェードを終了する
		m_fadeFlag = false;
		m_lerpBright = m_endBright;
	}
}

void Fade::Draw()
{
	int alpha = -m_lerpBright; //

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Fade::SetFadeIn(int millisecond)
{
	m_fadeFlag = true;
	m_time = millisecond;
	m_elapsedTime = 0;

	// -255 → 0に遷移させる
	m_beginBright = -255;
	m_endBright = 0;
	m_lerpBright = m_beginBright;
}

void Fade::SetFadeOut(int millisecond)
{
	m_fadeFlag = true;
	m_time = millisecond;
	m_elapsedTime = 0;

	// 0 → -255に遷移させる
	m_beginBright = 0;
	m_endBright = -255;
	m_lerpBright = m_beginBright;
}
