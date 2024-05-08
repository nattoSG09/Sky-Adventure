#pragma once

// インクルード
#include "../../Engine/GameObject/GameObject.h"

// `スプラッシュシーン`を管理するクラス
class SplashScene : public GameObject
{
private:
	int imageHandle_;	// 画像番号
	float count_;		// 経過時間

public:
	// コンストラクタ
	SplashScene(GameObject* _parent);

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 開放
	void Release() override{}
};

