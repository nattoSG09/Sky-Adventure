#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

// �O���錾
class Stage;

// `�e�X�g�V�[�����Ǘ�����N���X`
class TestScene : public GameObject
{
private:
	int imageHandle_;	// �摜�ԍ�

public:
	// �R���X�g���N�^
	TestScene(GameObject* parent);

	// ������
	void Initialize() override;

	// �X�V
	void Update() override{}

	// �`��
	void Draw() override{}

	// �J��
	void Release() override{}
};