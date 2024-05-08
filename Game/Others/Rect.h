#pragma once

// インクルード
#include <DirectXMath.h>

// using宣言
using namespace DirectX;

// `四角形`を構成するクラス
class Rect
{
public:
	XMFLOAT2 center_;	// 中心
	float width_;		// 横幅
	float height_;		// 縦幅

public:
	// コンストラクタ
	Rect();
	Rect(float _cx, float _cy, float _width, float _height);
	Rect(XMFLOAT2 _center, float _width, float _height);

	// `点ｐ`が四角形の範囲内に含まれているかどうかを返す
	bool ContainsPoint(float _px, float _py);
	bool ContainsPoint(XMFLOAT2 _point);
};

