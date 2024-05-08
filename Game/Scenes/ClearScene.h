#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

// `�N���A�V�[��`���Ǘ�����N���X
class ClearScene : public GameObject
{
private:
	int imageHandle_;	// �摜�ԍ�

public:
	// �R���X�g���N�^
	ClearScene(GameObject* _parent);

	// ������
	void Initialize() override;

	// �X�V
	void Update() override;

	// �`��
	void Draw() override;

	// �J��
	void Release() override;
};

