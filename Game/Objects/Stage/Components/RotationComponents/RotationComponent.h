#pragma once

// インクルード
#include "../Component.h"

// 全軸回転を行うコンポーネントのクラス
class RotationComponent : public Component
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_holeder">保有者</param>
	RotationComponent(StageObject* _holeder, Component* _parent);
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	
	/// <summary>
	/// 開放
	/// </summary>
	void Release() override;

	/// <summary>
	/// 保存
	/// </summary>
	/// <param name="_saveObj">保存情報</param>
	void Save(json& _saveObj) override;
	
	/// <summary>
	/// 読込
	/// </summary>
	/// <param name="_loadObj">読込情報</param>
	void Load(json& _loadObj) override;
	
	/// <summary>
	/// ImGui表示
	/// </summary>
	void DrawData() override;
};

