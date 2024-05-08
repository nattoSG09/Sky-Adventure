#include "RotationYComponent.h"

// �C���N���[�h
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"

RotationYComponent::RotationYComponent(StageObject* _holeder, Component* _parent)
	:Component(_holeder,"RotationYComponent",RotationY,_parent),angle_(0),rotateSpeed_(0)
{
}

void RotationYComponent::Initialize()
{
}

void RotationYComponent::Update()
{
	// �x���ɉ�]������
	angle_ += rotateSpeed_;

	// ��]���x�̒l�����̒l�ł��� ���� �p�x��360�x�����������A�p�x���O�ɖ߂�
	if (!std::signbit(rotateSpeed_) && angle_ >= 360.f)angle_ = 0.f;

	// ��]���x�̒l�����̒l�ł��� ���� �p�x��-360�x�����������A�p�x���O�ɖ߂�
	if (std::signbit(rotateSpeed_) && angle_ <= -360.f)angle_ = 0.f;

	// ��]���x���O�ł͂Ȃ����A�p�x��K��������
	if (rotateSpeed_ != 0.f)holder_->SetRotateY(angle_);
}

void RotationYComponent::Release()
{
}

void RotationYComponent::Save(json& _saveObj)
{
	// ��]���x��ۑ�
	_saveObj["rotateSpeed_"] = rotateSpeed_;

	// �p�x��ۑ�
	_saveObj["angle_"] = angle_;
}

void RotationYComponent::Load(json& _loadObj)
{
	// ��]���x��Ǎ�
	rotateSpeed_ = _loadObj["rotateSpeed_"];

	// �p�x��Ǎ�
	angle_ = _loadObj["angle_"];
}

void RotationYComponent::DrawData()
{
	// ��]���x��\��
	ImGui::DragFloat("rotateSpeed_", &rotateSpeed_,0.1f);
}