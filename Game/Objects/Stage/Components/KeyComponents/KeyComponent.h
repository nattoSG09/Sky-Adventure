#pragma once
#include "../Component.h"

#include <DirectXMath.h>
using namespace DirectX;

// X方向に平行移動を行うコンポーネントのクラス
class KeyComponent : public Component
{
private:
	bool isHit_;					// 接触したかどうか
	float count_;					// 時間経過
	XMFLOAT3 prevContactPosition_;	// 接触した時点での位置

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_holeder">保有者</param>
	KeyComponent(StageObject* _holeder, Component* _parent);

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

	void OnCollision(GameObject* _target) override;

};

