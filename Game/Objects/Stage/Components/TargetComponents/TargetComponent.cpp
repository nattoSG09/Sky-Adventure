#include "TargetComponent.h"
#include "../../Stage.h"
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../../Player/Player.h"

TargetComponent::TargetComponent(StageObject* _holder, Component* _parent)
	:Component(_holder,"TargetComponent",Target,_parent),hp_(100)

{
}

void TargetComponent::Initialize()
{
	holder_->AddCollider(new SphereCollider(XMFLOAT3(0, 0, 0), 1.2f));
}

void TargetComponent::Update()
{
	if (hp_ <= 0)((Stage*)holder_->FindObject("Stage"))->DeleteStageObject(holder_);
}

void TargetComponent::Release()
{
}

void TargetComponent::Save(json& _saveObj)
{
	_saveObj["hp_"] = hp_;
}

void TargetComponent::Load(json& _loadObj)
{
	hp_ = _loadObj["hp_"];
}

void TargetComponent::DrawData()
{
	ImGui::DragFloat("hp_", &hp_, 0.1f);
}

void TargetComponent::ReceiveDamage(float _damage)
{
	hp_ -= _damage;
}

void TargetComponent::OnCollision(GameObject* _target)
{
	// ƒvƒŒƒCƒ„[‚ª`myObject_`‚ÉG‚ê‚½‚ç...
	if (_target->GetObjectName() == "Player") {

		((Player*)_target)->Dead();
	}
}
