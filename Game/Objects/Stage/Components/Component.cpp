#include "Component.h"

// インクルード
#include "../../../../Engine/ImGui/imgui.h"
#include "../StageObject.h"

// 作成したコンポーネントのインクルード
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
	// 自身の初期化
	this->Initialize();

	// 子コンポーネントの初期化
	for (auto comp : childComponents_)comp->ChildIntialize();
}

void Component::ChildUpdate()
{
	// 自身の更新
	this->Update();

	// 子コンポーネントの更新
	for (auto comp : childComponents_)comp->ChildUpdate();
}

void Component::ChildRelease()
{
	// 子コンポーネントの開放
	for (auto comp : childComponents_) comp->ChildRelease();
	childComponents_.clear();
	
	// 自身の開放
	this->Release();
}

void Component::ChildDrawData()
{
	if (ImGui::TreeNode(this->name_.c_str())) {

		if (parentComponent_ == nullptr) {
			ImGui::SameLine();
			if (ImGui::SmallButton("Delete")) holder_->DeleteComponent(this);
		}

		// 自身の情報を描画
		this->DrawData();

		// 子コンポーネントの情報を描画
		if (childComponents_.empty() == false) {
			if (ImGui::TreeNode("childComponents_")) {
				// 子コンポーネントの情報を描画
				for (auto comp : childComponents_) comp->ChildDrawData();

				ImGui::TreePop();
			}
		}
		ImGui::TreePop();

	}
}

void Component::ChildSave(json& _saveObj)
{
	// 自身の情報を保存
	_saveObj["type_"] = type_;
	this->Save(_saveObj);

	// 子コンポーネントの情報を保存
	for (auto comp : childComponents_)comp->ChildSave(_saveObj["childComponents_"][comp->GetName()]);
}

void Component::ChildLoad(json& _loadObj)
{
	// 自身の情報を保存
	this->Load(_loadObj);

	// 子コンポーネントの情報を読込
	for (auto comp : childComponents_)comp->ChildLoad(_loadObj["childComponents_"][comp->GetName()]);
}

void Component::ChildOnCollision(GameObject* _target)
{
	// 自身のあたり判定
	this->OnCollision(_target);

	// 子コンポーネントのあたり判定
	for (auto comp : childComponents_)comp->ChildOnCollision(_target);
}

bool Component::AddChildComponent(Component* _comp)
{
	// ヌルチェック
	if (_comp == nullptr)
		return false;

	// 初期化
	_comp->ChildIntialize();

	// コンポーネントのリストに追加する
	childComponents_.push_back(_comp);
	return true;
}

bool Component::DeleteChildComponent(Component* _comp)
{
	// イテレータに、"childComponents_"内で探したいデータを登録
	auto it = std::find(childComponents_.begin(), childComponents_.end(), _comp);

	// イテレータがリストの最後まで到達したら関数終了
	if (it == childComponents_.end())
		return false;

	// イテレータのコンポーネントを消す
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

	// タイプ(識別番号)にしたがってコンポーネントを作成
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
	// タイプ(識別番号)にしたがってコンポーネント名を返す
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
