#pragma once

// インクルード
#include "../../Engine/GameObject/GameObject.h"

// `クリアシーン`を管理するクラス
class ClearScene : public GameObject
{
private:
	int imageHandle_;	// 画像番号

public:
	// コンストラクタ
	ClearScene(GameObject* _parent);

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 開放
	void Release() override;
};

