#include "StageObject.h"

// インクルード
#include "../../../Engine/ResourceManager/Model.h"
#include "../../../Engine/ImGui/imgui.h"
#include "Stage.h"

// マクロ
#define REFERENCE_XMFLOAT3(p) p.x,p.y,p.z // XMFLOAT3型の変数をコンマ区切りで表示する

StageObject::StageObject(string _name, string _modelFilePath, GameObject* _parent)
	:GameObject(_parent,_name),modelFilePath_(_modelFilePath),modelHandle_(-1),myComponents_()
{
}

bool StageObject::AddComponent(Component* _comp)
{
	// ヌルチェック
	if (_comp == nullptr) return false;

	// 初期化
	_comp->ChildIntialize();

	// コンポーネントのvectorに追加する
	myComponents_.push_back(_comp); 
	return true;
}

bool StageObject::DeleteComponent(Component* _comp)
{
	// イテレータに、"myComponents_"内で探したいデータを登録
	auto it = std::find(myComponents_.begin(), myComponents_.end(), _comp);

	// イテレータがリストの最後まで到達したら関数終了
	if (it == myComponents_.end()) return false;

	// イテレータのコンポーネントを消す
	myComponents_.erase(it); return true;
}

bool StageObject::DeleteAllComponent()
{
	// リスト内にある要素をすべて削除
	myComponents_.clear();return true;
}

bool StageObject::FindComponent(ComponentType _type)
{
	for (auto comp : myComponents_) {
		if (comp->GetType() == _type || comp->FindChildComponent(_type))return true;
	}
	return false;
}

Component* StageObject::GetComponent(ComponentType _type)
{
	Component* ret = nullptr;

	for (auto comp : myComponents_) {
		
		if (comp->GetType() == _type)ret = comp;

		// 子コンポーネントの取得は不可
		if (comp->FindChildComponent(_type))ret = comp->GetChildComponent(_type);
	}
	return ret;
}

void StageObject::Initialize()
{
	// モデルの読み込み
	modelHandle_ = Model::Load(modelFilePath_);
	assert(modelHandle_ >= 0);

	// 保有するコンポーネントの初期化処理
	for (auto comp : myComponents_)comp->ChildIntialize();
}

void StageObject::Update()
{
	// 保有するコンポーネントの更新処理
	for (auto comp : myComponents_)comp->ChildUpdate();
}

void StageObject::Draw()
{
	// モデルの描画
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);
}

void StageObject::Release()
{
	// 保有するコンポーネントの開放処理
	for (auto comp : myComponents_) comp->ChildRelease();
	myComponents_.clear();
}

void StageObject::OnCollision(GameObject* _target)
{
	for (auto comp : myComponents_)comp->ChildOnCollision(_target);
}

void StageObject::Save(json& _saveObj)
{
	// 自身の変形行列情報を保存
	_saveObj["position_"] = { REFERENCE_XMFLOAT3(transform_.position_) };
	_saveObj["rotate_"] = { REFERENCE_XMFLOAT3(transform_.rotate_) };
	_saveObj["scale_"] = { REFERENCE_XMFLOAT3(transform_.scale_)};
	
	// 自身のモデルのファイルパスを保存
	_saveObj["modelFilePath_"] = modelFilePath_;

	// コンポーネント情報を保存
	for (auto comp : myComponents_)comp->ChildSave(_saveObj["myComponents_"][comp->GetName()]);
}

void StageObject::Load(json& _loadObj)
{
	// 現在のすべてのコンポーネント情報を削除
	this->DeleteAllComponent();

	// 変形行列情報を読込
	transform_.position_ = { _loadObj["position_"][0].get<float>(),_loadObj["position_"][1].get<float>(), _loadObj["position_"][2].get<float>() };
	transform_.rotate_ = { _loadObj["rotate_"][0].get<float>(),_loadObj["rotate_"][1].get<float>(), _loadObj["rotate_"][2].get<float>() };
	transform_.scale_ = { _loadObj["scale_"][0].get<float>(),_loadObj["scale_"][1].get<float>(), _loadObj["scale_"][2].get<float>() };

	// モデルのファイルパスを読込
	modelFilePath_ = _loadObj["modelFilePath_"];

	// コンポーネントインスタンスを生成
	for (auto& obj : _loadObj["myComponents_"]) {
		
		// コンポーネントを生成
		Component* comp = CreateComponent(obj["type_"], this);

		// 子コンポーネントを生成
		for (auto& child : obj["childComponents_"])comp->AddChildComponent(CreateComponent(child["type_"], this,comp));

		// "myComponents_"に追加
		this->AddComponent(comp);
	}

	// コンポーネント情報を読込
	for (auto comp : myComponents_)comp->ChildLoad(_loadObj["myComponents_"][comp->GetName()]);
}

void StageObject::DrawData()
{
	ImGui::Text(this->objectName_.c_str());
	ImGui::SameLine();

	if (ImGui::SmallButton("delete")) ((Stage*)FindObject("Stage"))->DeleteStageObject(this);
	ImGui::Separator();

	if (ImGui::TreeNode("name")) {
		ImGui::Text("Current name : %s", this->objectName_.c_str());
		char buffer[256] = "";
		if (ImGui::InputTextWithHint("##Input", "Input New name...", buffer, IM_ARRAYSIZE(buffer)))
			this->objectName_ = buffer;
		ImGui::TreePop();
	}

	// 自身の変形情報を描画
	if (ImGui::TreeNode("transform_")) {
		ImGui::DragFloat3("position_", &transform_.position_.x, 0.1f);
		ImGui::DragFloat3("rotate_", &transform_.rotate_.x, 1.f,-360.f,360.f);
		ImGui::DragFloat3("scale_", &transform_.scale_.x, 0.1f,0.f,LONG_MAX);
		ImGui::TreePop();
	}

	// 保有するコンポーネントの情報を描画
	if (ImGui::TreeNode("myComponents_")) {
		ImGui::SameLine();

		// `コンポーネント追加`ボタンを描画
		if (ImGui::SmallButton("AddComponent")) showAddComponentWindow_ = true;

		// 保有するコンポーネントの情報を描画
		for (auto comp : myComponents_)comp->ChildDrawData();
		ImGui::TreePop();
	}

	// 追加できるコンポーネントリストのウィンドウを描画
	if (showAddComponentWindow_) {
		ImGui::Begin("Components that can be added", &showAddComponentWindow_); {
			for (int i = 0; i < ComponentType::Max; i++) {
				if (ImGui::Selectable(ComponentTypeToString((ComponentType)i).c_str(), selectedComponentType_ == i)) {
					
					// 選択しているコンポーネントタイプを代入
					selectedComponentType_ = (ComponentType)i;

					// 選択されたコンポーネントタイプのコンポーネントを作成し追加
					this->AddComponent(CreateComponent(selectedComponentType_, this));
					
					// ウィンドウを閉じる
					showAddComponentWindow_ = false;
				}
			}
		}
		ImGui::End();
	}
}

StageObject* CreateStageObject(string _name, string _modelFilePath, GameObject* _parent)
{
	// オブジェクトのインスタンスを生成
	StageObject* obj = new StageObject(_name, _modelFilePath, _parent);
	
	// 初期化
	obj->Initialize();
	
	// 親オブジェクトのリストに追加
	if (_parent != nullptr)_parent->PushBackChild(obj);
	
	return obj;
}