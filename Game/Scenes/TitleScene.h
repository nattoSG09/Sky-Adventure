#pragma once

// インクルード
#include "../../Engine/GameObject/GameObject.h"

// `タイトルシーン`を管理するクラス
class TitleScene : public GameObject
{
private:
	int imageHandle_;	// 画像番号

public:
	// コンストラクタ
	TitleScene(GameObject* _parent);
	
	// 初期化
	void Initialize() override;
	
	// 更新
	void Update() override;
	
	// 描画
	void Draw() override;
	
	// 開放
	void Release() override{}
};

