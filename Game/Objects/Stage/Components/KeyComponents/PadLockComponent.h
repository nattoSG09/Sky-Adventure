#pragma once
#include "../Component.h"

#include <DirectXMath.h>
using namespace DirectX;

// X方向に平行移動を行うコンポーネントのクラス
class PadLockComponent : public Component
{
private:
	bool isLock_;
	float count_;
public:
	PadLockComponent(StageObject* _holeder, Component* _parent);
	void Initialize() override;
	void Update() override;
	void Release() override;

	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;

	void DrawData() override;
	void OnCollision(GameObject* _target) override;

	void Open() { isLock_ = false; }
	void Lock() { isLock_ = true; }
};

