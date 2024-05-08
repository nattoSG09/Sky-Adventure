#pragma once

// �C���N���[�h
#include <DirectXMath.h>

// using�錾
using namespace DirectX;

// `�l�p�``���\������N���X
class Rect
{
public:
	XMFLOAT2 center_;	// ���S
	float width_;		// ����
	float height_;		// �c��

public:
	// �R���X�g���N�^
	Rect();
	Rect(float _cx, float _cy, float _width, float _height);
	Rect(XMFLOAT2 _center, float _width, float _height);

	// `�_��`���l�p�`�͈͓̔��Ɋ܂܂�Ă��邩�ǂ�����Ԃ�
	bool ContainsPoint(float _px, float _py);
	bool ContainsPoint(XMFLOAT2 _point);
};

