#include "HoverComponent.h"

#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"

HoverComponent::HoverComponent(StageObject* _holder, Component* _parent)
	:Component(_holder,"HoverComponent",Hover,_parent),direction_(0),speed_(0),min_(0),max_(0)
{
}

void HoverComponent::Initialize()
{
	tmpPositionY_ = holder_->GetPosition().y;
}

void HoverComponent::Update()
{
	XMFLOAT3 myPosition = holder_->GetPosition();

	if (myPosition.y <= (tmpPositionY_ + min_)) {
		direction_ = speed_;
	}
	else if (myPosition.y >= (tmpPositionY_ + max_)) {
		direction_ = -speed_;
	}

	myPosition.y += direction_;

	holder_->SetPosition(myPosition);
}

void HoverComponent::Release()
{
}

void HoverComponent::Save(json& _saveObj)
{
	_saveObj["speed_"] = speed_;
	_saveObj["min_"] = min_;
	_saveObj["max_"] = max_;
}

void HoverComponent::Load(json& _loadObj)
{
	speed_ = _loadObj["speed_"];
	min_ = _loadObj["min_"];
	max_ = _loadObj["max_"];
}

void HoverComponent::DrawData()
{
	ImGui::DragFloat("speed_", &speed_, 0.1f);
	ImGui::DragFloat("min_", &min_, 0.1f);
	ImGui::DragFloat("max_", &max_, 0.1f);
}
