#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

// `�^�C�g���V�[��`���Ǘ�����N���X
class TitleScene : public GameObject
{
private:
	int imageHandle_;	// �摜�ԍ�

public:
	// �R���X�g���N�^
	TitleScene(GameObject* _parent);
	
	// ������
	void Initialize() override;
	
	// �X�V
	void Update() override;
	
	// �`��
	void Draw() override;
	
	// �J��
	void Release() override{}
};

