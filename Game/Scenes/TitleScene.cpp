#include "TitleScene.h"

// インクルード
#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ResourceManager/Image.h"
#include "../../Engine/SceneManager.h"

// 定数
namespace {
	const float IMAGE_REDUCTION_RATE = 0.67f;	// 画像の縮小倍率
}

TitleScene::TitleScene(GameObject* _parent)
	:GameObject(_parent,"TitleScene"),imageHandle_(-1)
{
}

void TitleScene::Initialize()
{
	// 画像の読込
	imageHandle_ = Image::Load("Images/title_Image.png");
	assert(imageHandle_ >= 0);

	// 画像サイズを指定
	transform_.scale_ = { IMAGE_REDUCTION_RATE,IMAGE_REDUCTION_RATE,1.f };
}

void TitleScene::Update()
{
	// スペースキーを押す事でシーンを切り替える
	if (Input::IsKeyDown(DIK_SPACE))((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID_TEST, TID_BLACKOUT);
}

void TitleScene::Draw()
{
	// 画像の描画
	Image::SetTransform(imageHandle_, transform_);
	Image::Draw(imageHandle_);
}
