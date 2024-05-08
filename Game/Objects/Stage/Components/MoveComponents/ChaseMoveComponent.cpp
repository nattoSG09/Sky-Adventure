#include "ChaseMoveComponent.h"

#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../../Player/Player.h"
#include "../../../../../Engine/ResourceManager/VFX.h"

ChaseMoveComponent::ChaseMoveComponent(StageObject* _holder, Component* _parent)
	:Component(_holder,"ChaseMoveComponent",ChaseMove,_parent), recognitionDistance_(0.f,0.f,1.f),speed_(0)
{
}

void ChaseMoveComponent::Initialize()
{
}

void ChaseMoveComponent::Update()
{
	// �ʒu�𔽉f
	recognitionDistance_.center_.x = (holder_->GetPosition().x + center_.x);
	recognitionDistance_.center_.y = (holder_->GetPosition().y + center_.y);

	// �v���C���[�̏����擾
	Player* player = (Player*)holder_->FindObject("Player");
	if (player == nullptr)return;

	// �͈͓��ɍ�������B�B
	if (recognitionDistance_.ContainsPoint(player->GetPosition().x, player->GetPosition().y)) {

		XMFLOAT3 myPosition = holder_->GetPosition();
		XMFLOAT3 playerPosition = player->GetPosition();

		// this -> player�@�Ɍ������������擾
		XMVECTOR dir = XMVector3Normalize(XMVectorSetY(XMLoadFloat3(&playerPosition) - XMLoadFloat3(&myPosition),0));

		XMStoreFloat3(&myPosition, XMLoadFloat3(&myPosition) + (dir * speed_));

		holder_->SetPosition(myPosition);
	}

}

void ChaseMoveComponent::Release()
{
}

void ChaseMoveComponent::Save(json& _saveObj)
{
	_saveObj["recognitionDistance_"]["center_"] = { center_.x,center_.y };
	_saveObj["recognitionDistance_"]["radius_"] = recognitionDistance_.radius_;
	_saveObj["speed_"] = speed_;
}

void ChaseMoveComponent::Load(json& _loadObj)
{
	center_ = { _loadObj["recognitionDistance_"]["center_"][0].get<float>(),_loadObj["recognitionDistance_"]["center_"][1].get<float>() };
	recognitionDistance_.radius_ = _loadObj["recognitionDistance_"]["radius_"];
	speed_ = _loadObj["speed_"];
}

void ChaseMoveComponent::DrawData()
{
	ImGui::Text("recognitionDistance_");
	ImGui::Separator();
	ImGui::DragFloat2("center_", &center_.x, 0.1f);
	ImGui::DragFloat("radius_", &recognitionDistance_.radius_, 0.1f);
	ImGui::DragFloat("speed_", &speed_, 0.1f);

}
