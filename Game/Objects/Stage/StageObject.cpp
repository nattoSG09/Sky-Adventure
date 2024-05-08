#include "StageObject.h"

// �C���N���[�h
#include "../../../Engine/ResourceManager/Model.h"
#include "../../../Engine/ImGui/imgui.h"
#include "Stage.h"

// �}�N��
#define REFERENCE_XMFLOAT3(p) p.x,p.y,p.z // XMFLOAT3�^�̕ϐ����R���}��؂�ŕ\������

StageObject::StageObject(string _name, string _modelFilePath, GameObject* _parent)
	:GameObject(_parent,_name),modelFilePath_(_modelFilePath),modelHandle_(-1),myComponents_()
{
}

bool StageObject::AddComponent(Component* _comp)
{
	// �k���`�F�b�N
	if (_comp == nullptr) return false;

	// ������
	_comp->ChildIntialize();

	// �R���|�[�l���g��vector�ɒǉ�����
	myComponents_.push_back(_comp); 
	return true;
}

bool StageObject::DeleteComponent(Component* _comp)
{
	// �C�e���[�^�ɁA"myComponents_"���ŒT�������f�[�^��o�^
	auto it = std::find(myComponents_.begin(), myComponents_.end(), _comp);

	// �C�e���[�^�����X�g�̍Ō�܂œ��B������֐��I��
	if (it == myComponents_.end()) return false;

	// �C�e���[�^�̃R���|�[�l���g������
	myComponents_.erase(it); return true;
}

bool StageObject::DeleteAllComponent()
{
	// ���X�g���ɂ���v�f�����ׂč폜
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

		// �q�R���|�[�l���g�̎擾�͕s��
		if (comp->FindChildComponent(_type))ret = comp->GetChildComponent(_type);
	}
	return ret;
}

void StageObject::Initialize()
{
	// ���f���̓ǂݍ���
	modelHandle_ = Model::Load(modelFilePath_);
	assert(modelHandle_ >= 0);

	// �ۗL����R���|�[�l���g�̏���������
	for (auto comp : myComponents_)comp->ChildIntialize();
}

void StageObject::Update()
{
	// �ۗL����R���|�[�l���g�̍X�V����
	for (auto comp : myComponents_)comp->ChildUpdate();
}

void StageObject::Draw()
{
	// ���f���̕`��
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);
}

void StageObject::Release()
{
	// �ۗL����R���|�[�l���g�̊J������
	for (auto comp : myComponents_) comp->ChildRelease();
	myComponents_.clear();
}

void StageObject::OnCollision(GameObject* _target)
{
	for (auto comp : myComponents_)comp->ChildOnCollision(_target);
}

void StageObject::Save(json& _saveObj)
{
	// ���g�̕ό`�s�����ۑ�
	_saveObj["position_"] = { REFERENCE_XMFLOAT3(transform_.position_) };
	_saveObj["rotate_"] = { REFERENCE_XMFLOAT3(transform_.rotate_) };
	_saveObj["scale_"] = { REFERENCE_XMFLOAT3(transform_.scale_)};
	
	// ���g�̃��f���̃t�@�C���p�X��ۑ�
	_saveObj["modelFilePath_"] = modelFilePath_;

	// �R���|�[�l���g����ۑ�
	for (auto comp : myComponents_)comp->ChildSave(_saveObj["myComponents_"][comp->GetName()]);
}

void StageObject::Load(json& _loadObj)
{
	// ���݂̂��ׂẴR���|�[�l���g�����폜
	this->DeleteAllComponent();

	// �ό`�s�����Ǎ�
	transform_.position_ = { _loadObj["position_"][0].get<float>(),_loadObj["position_"][1].get<float>(), _loadObj["position_"][2].get<float>() };
	transform_.rotate_ = { _loadObj["rotate_"][0].get<float>(),_loadObj["rotate_"][1].get<float>(), _loadObj["rotate_"][2].get<float>() };
	transform_.scale_ = { _loadObj["scale_"][0].get<float>(),_loadObj["scale_"][1].get<float>(), _loadObj["scale_"][2].get<float>() };

	// ���f���̃t�@�C���p�X��Ǎ�
	modelFilePath_ = _loadObj["modelFilePath_"];

	// �R���|�[�l���g�C���X�^���X�𐶐�
	for (auto& obj : _loadObj["myComponents_"]) {
		
		// �R���|�[�l���g�𐶐�
		Component* comp = CreateComponent(obj["type_"], this);

		// �q�R���|�[�l���g�𐶐�
		for (auto& child : obj["childComponents_"])comp->AddChildComponent(CreateComponent(child["type_"], this,comp));

		// "myComponents_"�ɒǉ�
		this->AddComponent(comp);
	}

	// �R���|�[�l���g����Ǎ�
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

	// ���g�̕ό`����`��
	if (ImGui::TreeNode("transform_")) {
		ImGui::DragFloat3("position_", &transform_.position_.x, 0.1f);
		ImGui::DragFloat3("rotate_", &transform_.rotate_.x, 1.f,-360.f,360.f);
		ImGui::DragFloat3("scale_", &transform_.scale_.x, 0.1f,0.f,LONG_MAX);
		ImGui::TreePop();
	}

	// �ۗL����R���|�[�l���g�̏���`��
	if (ImGui::TreeNode("myComponents_")) {
		ImGui::SameLine();

		// `�R���|�[�l���g�ǉ�`�{�^����`��
		if (ImGui::SmallButton("AddComponent")) showAddComponentWindow_ = true;

		// �ۗL����R���|�[�l���g�̏���`��
		for (auto comp : myComponents_)comp->ChildDrawData();
		ImGui::TreePop();
	}

	// �ǉ��ł���R���|�[�l���g���X�g�̃E�B���h�E��`��
	if (showAddComponentWindow_) {
		ImGui::Begin("Components that can be added", &showAddComponentWindow_); {
			for (int i = 0; i < ComponentType::Max; i++) {
				if (ImGui::Selectable(ComponentTypeToString((ComponentType)i).c_str(), selectedComponentType_ == i)) {
					
					// �I�����Ă���R���|�[�l���g�^�C�v����
					selectedComponentType_ = (ComponentType)i;

					// �I�����ꂽ�R���|�[�l���g�^�C�v�̃R���|�[�l���g���쐬���ǉ�
					this->AddComponent(CreateComponent(selectedComponentType_, this));
					
					// �E�B���h�E�����
					showAddComponentWindow_ = false;
				}
			}
		}
		ImGui::End();
	}
}

StageObject* CreateStageObject(string _name, string _modelFilePath, GameObject* _parent)
{
	// �I�u�W�F�N�g�̃C���X�^���X�𐶐�
	StageObject* obj = new StageObject(_name, _modelFilePath, _parent);
	
	// ������
	obj->Initialize();
	
	// �e�I�u�W�F�N�g�̃��X�g�ɒǉ�
	if (_parent != nullptr)_parent->PushBackChild(obj);
	
	return obj;
}