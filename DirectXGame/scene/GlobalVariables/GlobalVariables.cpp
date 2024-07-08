#include "GlobalVariables.h"

#include <assert.h>

#include <filesystem>
#include <fstream>

#include <Windows.h>

#include <imgui.h>
#include <json.hpp>

using json = nlohmann::json;
const std::string kDirectoryPath = "Resources/GlobalVariables/";

GlobalVariables *GlobalVariables::getInstance() {
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::Update() {
#ifdef _DEBUG
	if(!ImGui::Begin("Grobal Variables",nullptr,ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if(!ImGui::BeginMenuBar()) {
		return;
	}

	for(std::map<std::string,Group>::iterator groupItr = datas_.begin();
		groupItr != datas_.end();
		++groupItr) {

		const std::string &groupName = groupItr->first;

		if(!ImGui::BeginMenu(groupName.c_str())) {
			continue;
		}
		Group &group = groupItr->second;

		for(std::map<std::string,Item>::iterator itemItr = group.items.begin();
		itemItr != group.items.end();
		++itemItr) {

			const std::string &itemName = itemItr->first;
			Item &item = itemItr->second;
			if(std::holds_alternative<int32_t>(item.value)) {
				int32_t *valuePtr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(),valuePtr,0,100);
			} else if(std::holds_alternative<float>(item.value)) {
				float *valuePtr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(),valuePtr,-10,10);
			} else if(std::holds_alternative<Vector3>(item.value)) {
				Vector3 *valuePtr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(),reinterpret_cast<float *>(valuePtr),-10,10);
			}

		}

		ImGui::Text("\n");
		if(ImGui::Button("save")) {
			SaveFile(groupName);
			std::string message = std::format("{}.json saved",groupName);
			MessageBoxA(nullptr,message.c_str(),"GlobalVariables",0);
		}

		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
	ImGui::End();
#endif // _DEBUG
}

void GlobalVariables::CreateGroup(const std::string &groupName) {
	datas_[groupName];
}

void GlobalVariables::SaveFile(const std::string &groupName) {
	///========================================
	///保存項目をまとめる
	///========================================
	std::map<std::string,Group>::iterator groupItr = datas_.find(groupName);
	assert(groupItr != datas_.end());

	json root = json::object();
	root[groupName] = json::object();

	for(std::map<std::string,Item>::iterator itemItr = groupItr->second.items.begin();
		itemItr != groupItr->second.items.end(); ++itemItr) {
		const std::string &itemName = itemItr->first;
		Item &item = itemItr->second;

		if(std::holds_alternative<int32_t>(item.value)) {
			root[groupName][itemName] = std::get<int32_t>(item.value);
		} else if(std::holds_alternative<float>(item.value)) {
			root[groupName][itemName] = std::get<float>(item.value);
		} else if(std::holds_alternative<Vector3>(item.value)) {
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({value.x,value.y,value.z});
		}

		///========================================
		/// ファイルへ書き出す
		///========================================
		std::filesystem::path dir(kDirectoryPath);
		if(!std::filesystem::exists(dir)) {
			std::filesystem::create_directory(dir);
		}
		std::string filePath = kDirectoryPath + groupName + ".json";

		std::ofstream ofs;
		ofs.open(filePath);
		if(ofs.fail()) {
			std::string message = "Failed open data file for write.";
			MessageBoxA(nullptr,message.c_str(),"GrobalVariables",0);
			assert(0);
			return;
		}

		ofs << std::setw(4) << root << std::endl;
		ofs.close();
	}
}

void GlobalVariables::setValue(const std::string &groupName,const std::string &itemName,int32_t value) {
	Group &group = datas_[groupName];
	Item newItem {};
	newItem.value = value;
	group.items[itemName] = newItem;
}

void GlobalVariables::setValue(const std::string &groupName,const std::string &itemName,float value) {
	Group &group = datas_[groupName];
	Item newItem {};
	newItem.value = value;
	group.items[itemName] = newItem;
}

void GlobalVariables::setValue(const std::string &groupName,const std::string &itemName,Vector3 value) {
	Group &group = datas_[groupName];
	Item newItem {};
	newItem.value = value;
	group.items[itemName] = newItem;
}
