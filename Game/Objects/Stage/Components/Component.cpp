#include "Component.h"

// �C���N���[�h
#include "../../../../Engine/ImGui/imgui.h"
#include "../StageObject.h"

// �쐬�����R���|�[�l���g�̃C���N���[�h
#include "RotationComponents/RotationComponent.h"
#include "RotationComponents/RotationYComponent.h"
#include "RotationComponents/RotationXComponent.h"
#include "RotationComponents/RotationZComponent.h"
#include "MoveComponents/MoveXComponent.h"
#include "MoveComponents/ChaseMoveComponent.h"
#include "MoveComponents/HoverComponent.h"
#include "KeyComponents/KeyComponent.h"
#include "KeyComponents/PadLockComponent.h"
#include "WarpComponents/WarpComponent.h"
#include "TargetComponents/TargetComponent.h"

Component::Component(StageObject* _holder, string _name,ComponentType _type, Component* _parent)
    :holder_(_holder), name_(_name),type_(_type),childComponents_(),parentComponent_(_parent)
{
}

void Component::ChildIntialize()
{
	// ���g�̏�����
	this->Initialize();

	// �q�R���|�[�l���g�̏�����
	for (auto comp : childComponents_)comp->ChildIntialize();
}

void Component::ChildUpdate()
{
	// ���g�̍X�V
	this->Update();

	// �q�R���|�[�l���g�̍X�V
	for (auto comp : childComponents_)comp->ChildUpdate();
}

void Component::ChildRelease()
{
	// �q�R���|�[�l���g�̊J��
	for (auto comp : childComponents_) comp->ChildRelease();
	childComponents_.clear();
	
	// ���g�̊J��
	this->Release();
}

void Component::ChildDrawData()
{
	if (ImGui::TreeNode(this->name_.c_str())) {

		if (parentComponent_ == nullptr) {
			ImGui::SameLine();
			if (ImGui::SmallButton("Delete")) holder_->DeleteComponent(this);
		}

		// ���g�̏���`��
		this->DrawData();

		// �q�R���|�[�l���g�̏���`��
		if (childComponents_.empty() == false) {
			if (ImGui::TreeNode("childComponents_")) {
				// �q�R���|�[�l���g�̏���`��
				for (auto comp : childComponents_) comp->ChildDrawData();

				ImGui::TreePop();
			}
		}
		ImGui::TreePop();

	}
}

void Component::ChildSave(json& _saveObj)
{
	// ���g�̏���ۑ�
	_saveObj["type_"] = type_;
	this->Save(_saveObj);

	// �q�R���|�[�l���g�̏���ۑ�
	for (auto comp : childComponents_)comp->ChildSave(_saveObj["childComponents_"][comp->GetName()]);
}

void Component::ChildLoad(json& _loadObj)
{
	// ���g�̏���ۑ�
	this->Load(_loadObj);

	// �q�R���|�[�l���g�̏���Ǎ�
	for (auto comp : childComponents_)comp->ChildLoad(_loadObj["childComponents_"][comp->GetName()]);
}

void Component::ChildOnCollision(GameObject* _target)
{
	// ���g�̂����蔻��
	this->OnCollision(_target);

	// �q�R���|�[�l���g�̂����蔻��
	for (auto comp : childComponents_)comp->ChildOnCollision(_target);
}

bool Component::AddChildComponent(Component* _comp)
{
	// �k���`�F�b�N
	if (_comp == nullptr)
		return false;

	// ������
	_comp->ChildIntialize();

	// �R���|�[�l���g�̃��X�g�ɒǉ�����
	childComponents_.push_back(_comp);
	return true;
}

bool Component::DeleteChildComponent(Component* _comp)
{
	// �C�e���[�^�ɁA"childComponents_"���ŒT�������f�[�^��o�^
	auto it = std::find(childComponents_.begin(), childComponents_.end(), _comp);

	// �C�e���[�^�����X�g�̍Ō�܂œ��B������֐��I��
	if (it == childComponents_.end())
		return false;

	// �C�e���[�^�̃R���|�[�l���g������
	childComponents_.erase(it);
	return true;
}

bool Component::FindChildComponent(ComponentType _type)
{
	for (auto comp : childComponents_) if (comp->type_ == _type)return true;
	return false;
}

Component* Component::GetChildComponent(ComponentType _type)
{
	for (auto comp : childComponents_)if (comp->type_ == _type)return comp;
	return nullptr;
}

Component* CreateComponent(ComponentType _type, StageObject* _holder, Component* _parent)
{
	Component* comp = nullptr;

	// �^�C�v(���ʔԍ�)�ɂ��������ăR���|�[�l���g���쐬
	switch (_type)
	{
	case Rotation:comp = new RotationComponent(_holder,_parent);break;
	case RotationY:comp = new RotationYComponent(_holder, _parent); break;
	case RotationX:comp = new RotationXComponent(_holder, _parent); break;
	case RotationZ:comp = new RotationZComponent(_holder, _parent); break;
	case MoveX:comp = new MoveXComponent(_holder, _parent); break;
	case Key:comp = new KeyComponent(_holder, _parent); break;
	case PadLock:comp = new PadLockComponent(_holder, _parent); break;
	case Warp:comp = new WarpComponent(_holder, _parent); break;
	case Target:comp = new TargetComponent(_holder, _parent); break;
	case ChaseMove:comp = new ChaseMoveComponent(_holder, _parent); break;
	case Hover:comp = new HoverComponent(_holder, _parent); break;
	case Max:break;
	}
	return comp;
}

Component* CreateComponent(ComponentType _type, StageObject* _holder)
{
	return CreateComponent(_type,_holder,nullptr);
}

string ComponentTypeToString(ComponentType _type)
{
	// �^�C�v(���ʔԍ�)�ɂ��������ăR���|�[�l���g����Ԃ�
	switch (_type)
	{
	case Rotation:		return "RotationComponent";
	case RotationY:		return "RotationYComponent";
	case RotationX:		return "RotationXComponent";
	case RotationZ:		return "RotationZComponent";
	case MoveX:			return "MoveXComponent";
	case Key:			return "KeyComponent";
	case PadLock:		return "PadLockComponent";
	case Warp:			return "WarpComponent";
	case Target:		return "TargetComponent";
	case ChaseMove:		return "ChaseMoveComponent";
	case Hover:		return "HoverComponent";
	case Max:			return "None";
	}

	return "None";
}
