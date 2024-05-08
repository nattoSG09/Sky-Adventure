#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

// `�X�v���b�V���V�[��`���Ǘ�����N���X
class SplashScene : public GameObject
{
private:
	int imageHandle_;	// �摜�ԍ�
	float count_;		// �o�ߎ���

public:
	// �R���X�g���N�^
	SplashScene(GameObject* _parent);

	// ������
	void Initialize() override;

	// �X�V
	void Update() override;

	// �`��
	void Draw() override;

	// �J��
	void Release() override{}
};

