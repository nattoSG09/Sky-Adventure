#include "RotationComponent.h"

RotationComponent::RotationComponent(StageObject* _holeder, Component* _parent)
	:Component(_holeder,"RotationComponent",Rotation,_parent)
{
}

void RotationComponent::Initialize()
{
	// 各軸の回転用コンポーネントを追加
	if (FindChildComponent(RotationX) == false)AddChildComponent(CreateComponent(RotationX, holder_,this));
	if (FindChildComponent(RotationY) == false)AddChildComponent(CreateComponent(RotationY, holder_,this));
	if (FindChildComponent(RotationZ) == false)AddChildComponent(CreateComponent(RotationZ, holder_,this));
}

void RotationComponent::Update()
{
}

void RotationComponent::Release()
{
}

void RotationComponent::Save(json& _saveObj)
{
}

void RotationComponent::Load(json& _loadObj)
{
}

void RotationComponent::DrawData()
{
}
