#include "Stage.h"

// �C���N���[�h
#include "StageObject.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/DirectX/Direct3D.h"


namespace {
	// ��������̑S�Ă� "\\" �� "/" �ɒu������֐�
	void ReplaceBackslashes(string& str) {
		size_t found = str.find("\\");
		while (found != string::npos) {
			str.replace(found, 1, "/"); // "\\" �� "/" �ɒu������
			found = str.find("\\", found + 1); // ���� "\\" ����������
		}
	}

	// �J�����g�f�B���N�g������̑��΃p�X���擾����֐�
	std::string GetAssetsRelativePath(const std::string& absolutePath) {
		const std::string assetsFolder = "Assets\\";
		// "Assets\\" �̈ʒu������
		size_t assetsPos = absolutePath.find(assetsFolder);
		if (assetsPos != std::string::npos) {
			// "Assets\\" �̕������������A����ȍ~�̕������擾
			std::string relativePath = absolutePath.substr(assetsPos + assetsFolder.size());
			return relativePath;
		}
		else {
			// "Assets\\" ��������Ȃ��ꍇ�́A���̃t�@�C���p�X��Ԃ�
			return absolutePath;
		}
	}

}

Stage::Stage(GameObject* _parent)
	:GameObject(_parent,"Stage"),objects_()
{
}

void Stage::Initialize()
{
}

void Stage::Update()
{
}

void Stage::Draw()
{
#ifdef _DEBUG
	DrawWorldOutliner();
	DrawDatails();
#endif // _DEBUG

}

void Stage::Release()
{
}

void Stage::Save(json& _saveObj)
{
	// �I�u�W�F�N�g�Q��ۑ�
	for (auto obj : objects_)obj->Save(_saveObj[obj->GetObjectName()]);
}

void Stage::Load(json& _loadObj)
{
	// �X�e�[�W�I�u�W�F�N�g�����ׂč폜
	DeleteAllStageObject();

	for (auto it = _loadObj.begin(); it != _loadObj.end();++it) {
		
		// �I�u�W�F�N�g�̃C���X�^���X�𐶐�
		StageObject* obj = CreateStageObject(it.key(), it.value()["modelFilePath_"], this);
		
		// �I�u�W�F�N�g����Ǎ�
		obj->Load(it.value());

		// �I�u�W�F�N�g�����X�g�ɒǉ�
		AddStageObject(obj);
	}
}

void Stage::DrawWorldOutliner()
{
	// ImGui�ŕ\������E�B���h�E�̐ݒ���s��
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, 0));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));
	
	// �E�B���h�E��\��
	ImGui::Begin("World Outliner", NULL,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		// - ActionButtons ------------------------------------------ 
		ImGui::Text("Stage control Buttons");

		// `�ǉ�`�{�^����\��
		if (ImGui::Button("Add")) {
			// �ǉ�����I�u�W�F�N�g�̏�������ݒ�
			string name = "object" + std::to_string(objects_.size() + 1);

			//���݂̃J�����g�f�B���N�g�����o���Ă���
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
			string filePath{}; {
				// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
					ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
					ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
					ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
					ofn.lpstrFile[0] = '\0'; // ������
					ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
					ofn.lpstrFilter = TEXT("FBX�t�@�C��(*.fbx)\0*.fbx\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
					ofn.nFilterIndex = 1; // �����I������t�B���^�[
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
					ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
				}

				// �t�@�C����I������_�C�A���O�̕\��
				if (GetOpenFileName(&ofn) == TRUE) {
					// �t�@�C���p�X���擾
					filePath = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					filePath = GetAssetsRelativePath(filePath);

					// ���������"\\"��"/"�ɒu��
					ReplaceBackslashes(filePath);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// �쐬�𒆒f
					ImGui::End();
					return;
				}
			}

			// �I�u�W�F�N�g�̃��X�g�ɒǉ�
			this->AddStageObject(CreateStageObject(name, filePath, this));
		}
		ImGui::SameLine();

		// `�ۑ�`�{�^����\��
		if (ImGui::Button("Save")) {
			//���݂̃J�����g�f�B���N�g�����o���Ă���
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// �ۑ���̃t�@�C���p�X���擾
			string filePath{}; {
				// �u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
				OPENFILENAME ofn; {
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(OPENFILENAME);
					ofn.lpstrFilter = TEXT("objectData(*.json)\0*.json\0���ׂẴt�@�C��(*.*)\0*.*\0\0");
					char fileName[MAX_PATH] = "����.json";
					ofn.lpstrFile = fileName;
					ofn.nMaxFile = MAX_PATH;
					ofn.Flags = OFN_OVERWRITEPROMPT;
					ofn.lpstrDefExt = "json";
					ofn.nFilterIndex = 1; // �����I������t�B���^�[
					ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
				}

				// �t�@�C����ۑ�����_�C�A���O�̕\��
				if (GetSaveFileName(&ofn) == TRUE) {
					// �t�@�C���p�X���擾
					filePath = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					filePath = GetAssetsRelativePath(filePath);

					// ���������"\\"��"/"�ɒu��
					ReplaceBackslashes(filePath);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// �ۑ��𒆒f
					ImGui::End();
					return;
				}
			}

			// �ۑ��I�u�W�F�N�g���쐬
			json saveObj;

			// �I�u�W�F�N�g�Ƀf�[�^���i�[
			this->Save(saveObj);

			// �t�@�C���Ƀf�[�^��ۑ�
			JsonReader::Save(filePath, saveObj);
		}
		ImGui::SameLine();

		// `�Ǎ�`�{�^����\��
		if (ImGui::Button("Load")) {
			//���݂̃J�����g�f�B���N�g�����o���Ă���
			char defaultCurrentDir[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

			// �ǂݍ��ރt�@�C���̃p�X���擾
			string filePath{}; {
				// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
				OPENFILENAME ofn; {
					TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
					ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
					ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
					ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
					ofn.lpstrFile[0] = '\0'; // ������
					ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
					ofn.lpstrFilter = TEXT("JSON�t�@�C��(*.json)\0*.json\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
					ofn.nFilterIndex = 1; // �����I������t�B���^�[
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
					ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
				}

				// �t�@�C����I������_�C�A���O�̕\��
				if (GetOpenFileName(&ofn) == TRUE) {
					// �t�@�C���p�X���擾
					filePath = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					filePath = GetAssetsRelativePath(filePath);

					// ���������"\\"��"/"�ɒu��
					ReplaceBackslashes(filePath);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);
				}
				else {
					// �쐬�𒆒f
					ImGui::End();
					return;
				}
			}

			// �Ǎ��I�u�W�F�N�g���쐬
			json loadObj;

			// �t�@�C������f�[�^��Ǎ�
			JsonReader::Load(filePath, loadObj);

			// �I�u�W�F�N�g����f�[�^��Ǎ�
			this->Load(loadObj);
		}
		ImGui::SameLine();

		// `�폜`�{�^����\��
		if (ImGui::Button("Delete"))this->DeleteAllStageObject();

		// ��؂�ׁ̈A���C����\��
		ImGui::Separator();

		// - ObjectList ---------------------------------------------
		ImGui::BeginChild("ObjectList"); {
			// ���X�g��\��
			for (int i = 0; i < objects_.size(); ++i)
				if (ImGui::Selectable(objects_[i]->GetObjectName().c_str(), selectedIndex_ == i)) {
					selectedIndex_ = i;
				}
		}
		ImGui::EndChild();
		// ----------------------------------------------------------
	}
	ImGui::End();
}

void Stage::DrawDatails()
{
	// ImGui�ŕ\������E�B���h�E�̐ݒ���s��
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, Direct3D::screenHeight_ * 0.5f));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));

	// �E�B���h�E��\��
	ImGui::Begin("Datails", NULL,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	{
		// �I������Ă���I�u�W�F�N�g�̏ڍׂ�\��
		if (selectedIndex_ >= 0 && selectedIndex_ < objects_.size()) {

			objects_[selectedIndex_]->DrawData();
		}
		else ImGui::Text("No information to display");
	}
	ImGui::End();
}

void Stage::AddStageObject(StageObject* _obj)
{
	// ���X�g�ɒǉ�
	if (_obj != nullptr)objects_.push_back(_obj);
}

void Stage::DeleteStageObject(StageObject* _obj)
{
	// �I�u�W�F�N�g���폜����
	_obj->KillMe();

	// �I�u�W�F�N�g�̃C�e���[�^���擾����
	auto it = std::find(objects_.begin(), objects_.end(), _obj);

	// �C�e���[�^�����������ꍇ�A�x�N�^�[����폜����
	if (it != objects_.end()) objects_.erase(it);
}

void Stage::DeleteAllStageObject()
{
	// ���X�g���ɂ���v�f�����ׂč폜
	for (auto obj : objects_) obj->KillMe();
	objects_.clear();
}
