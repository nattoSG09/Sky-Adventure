#include "TitleScene.h"

// �C���N���[�h
#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ResourceManager/Image.h"
#include "../../Engine/SceneManager.h"

// �萔
namespace {
	const float IMAGE_REDUCTION_RATE = 0.67f;	// �摜�̏k���{��
}

TitleScene::TitleScene(GameObject* _parent)
	:GameObject(_parent,"TitleScene"),imageHandle_(-1)
{
}

void TitleScene::Initialize()
{
	// �摜�̓Ǎ�
	imageHandle_ = Image::Load("Images/title_Image.png");
	assert(imageHandle_ >= 0);

	// �摜�T�C�Y���w��
	transform_.scale_ = { IMAGE_REDUCTION_RATE,IMAGE_REDUCTION_RATE,1.f };
}

void TitleScene::Update()
{
	// �X�y�[�X�L�[���������ŃV�[����؂�ւ���
	if (Input::IsKeyDown(DIK_SPACE))((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID_TEST, TID_BLACKOUT);
}

void TitleScene::Draw()
{
	// �摜�̕`��
	Image::SetTransform(imageHandle_, transform_);
	Image::Draw(imageHandle_);
}
