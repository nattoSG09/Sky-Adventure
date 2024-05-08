#include "ClearScene.h"

// インクルード
#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ResourceManager/Image.h"
#include "../../Engine/SceneManager.h"


// 定数
namespace {
	const float IMAGE_REDUCTION_RATE = 0.67f;	// 画像の縮小倍率
}

ClearScene::ClearScene(GameObject* _parent)
	:GameObject(_parent,"ClearScene"),imageHandle_(-1)
{
}

void ClearScene::Initialize()
{
	// 画像の読込
	imageHandle_ = Image::Load("Images/clear_Image.png");
	assert(imageHandle_ >= 0);

	// 画像サイズを指定
	transform_.scale_ = { IMAGE_REDUCTION_RATE,IMAGE_REDUCTION_RATE,1.f };
}

void ClearScene::Update()
{
	// スペースキーを押す事でシーンを切り替える
	if(Input::IsKeyDown(DIK_SPACE))((SceneManager*)FindObject("SceneManager"))->ChangeScene(SCENE_ID_SPLASH, TID_BLACKOUT);

}

void ClearScene::Draw()
{
	// 画像の描画
	Image::SetTransform(imageHandle_, transform_);
	Image::Draw(imageHandle_);
}

void ClearScene::Release()
{
}
