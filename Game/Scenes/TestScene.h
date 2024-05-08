#pragma once

// インクルード
#include "../../Engine/GameObject/GameObject.h"

// 前方宣言
class Stage;

// `テストシーンを管理するクラス`
class TestScene : public GameObject
{
private:
	int imageHandle_;	// 画像番号

public:
	// コンストラクタ
	TestScene(GameObject* parent);

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override{}

	// 描画
	void Draw() override{}

	// 開放
	void Release() override{}
};