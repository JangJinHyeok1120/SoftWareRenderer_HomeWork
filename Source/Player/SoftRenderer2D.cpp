
#include "Precompiled.h"
#include "SoftRenderer.h"

// 그리드 그리기
void SoftRenderer::DrawGrid2D()
{
	// 그리드 색상
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// 가로 세로 라인 그리기
	ScreenPoint screenHalfSize = _ScreenSize.GetHalf();

	for (int x = screenHalfSize._X; x <= _ScreenSize._X; x += _Grid2DUnit)
	{
		_RSI->DrawFullVerticalLine(x, gridColor);
		if (x > screenHalfSize._X)
		{
			_RSI->DrawFullVerticalLine(2 * screenHalfSize._X - x, gridColor);
		}
	}

	for (int y = screenHalfSize._Y; y <= _ScreenSize._Y; y += _Grid2DUnit)
	{
		_RSI->DrawFullHorizontalLine(y, gridColor);
		if (y > screenHalfSize._Y)
		{
			_RSI->DrawFullHorizontalLine(2 * screenHalfSize._Y - y, gridColor);
		}
	}

	// 월드 축 그리기
	_RSI->DrawFullHorizontalLine(screenHalfSize._Y, LinearColor::Red);
	_RSI->DrawFullVerticalLine(screenHalfSize._X, LinearColor::Green);
}


// 게임 로직
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// 경과 시간 저장
	static float elapsedTime = 0.f;
	elapsedTime += InDeltaSeconds;

	// 1초에 0~1 범위로 진동하는 사인 파동 만들기 
	twoPiUnit = elapsedTime * Math::TwoPI;
	float sinWave = sinf(twoPiUnit);
	float sinWave01 = (sinWave + 1.f) * 0.5f;

	_Transform.SetPosition(Vector2(0.f, sinWave));

	// 색상 변경
	_CurrentColor._R = 1.f - sinWave01;
	_CurrentColor._G = 0.f;
	_CurrentColor._B = sinWave01;

	// 크기 행렬 생성
	static float minScale = 0.5f;
	static float maxScale = 2.0f;
	static float scaleLength = maxScale - minScale;
	float currentScale = sinWave01 * scaleLength + minScale;
	Matrix2x2 scaleMatrix = Matrix2x2::Identity * currentScale;

	_Transform.SetScale(Vector2(100.f, 100.0f));

	// 회전 행렬 생성
	static float rotateSpeed = 60.f;
	static float currentAngle = 0.f;
	currentAngle += rotateSpeed * InDeltaSeconds;
	float currentRadian = Math::Deg2Rad(currentAngle);
	float sin = sinf(currentRadian);
	float cos = cosf(currentRadian);
	Vector2 xAxis(cos, sin);
	Vector2 yAxis(-sin, cos);
	Matrix2x2 rotationMatrix(xAxis, yAxis);

	_Transform.SetRotation(currentAngle);

	// 두 행렬을 조합하기. 순서에 주의할 것 
	_DeltaTransform = rotationMatrix * scaleMatrix;

	Vector3 povec = _Transform.GetModelingMatrix() * Vector3(1, 1, 1);
	_CurrentPosition = povec.ToVector2();
}

// 렌더링 로직
void SoftRenderer::Render2D()
{
	// 격자 그리기
	DrawGrid2D();

	for (int i = 0; i < 36; i++)
	{
		float sinwave = sinf(twoPiUnit + 10 * i);

		Vector2 pos = Vector2(_CurrentPosition._X + i * 10.0f, _CurrentPosition._Y + sinwave * 100.0f);


			_RSI->DrawPoint(pos, _CurrentColor);
			_RSI->DrawPoint(pos + Vector2::UnitX, _CurrentColor);
			_RSI->DrawPoint(pos - Vector2::UnitX, _CurrentColor);
			_RSI->DrawPoint(pos + Vector2::UnitY, _CurrentColor);
			_RSI->DrawPoint(pos - Vector2::UnitY, _CurrentColor);
		
	}

}

