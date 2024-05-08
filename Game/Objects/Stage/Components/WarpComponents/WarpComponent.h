#pragma once

#include "../Component.h"

#include <DirectXMath.h>
using namespace DirectX;

class WarpComponent : public Component
{
private:
	XMFLOAT3 destination_;	// ワープ先の位置
	bool isRide_;			// ワープ地点に乗っているかどうか
	float count_;			// 経過時間
public:
	WarpComponent(StageObject* _holder, Component* _parent);
	
	void Initialize() override;
	void Update() override;
	void Release() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;

	void DrawData() override;

	void OnCollision(GameObject* _target) override;
};

