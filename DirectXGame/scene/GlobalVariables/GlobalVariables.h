#pragma once

#include <map>

#include <variant>

#include <iostream>

#include <assert.h>

#include "Vector3.h"
#include <string>

class GlobalVariables{
public:
	static GlobalVariables *getInstance();

	void Update();

	void CreateGroup(const std::string &groupName);

	void LoadAllFile();
	void LoadFile(const std::string &groupName);
	void SaveFile(const std::string &groupName);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables &) = delete;
	GlobalVariables *operator=(GlobalVariables &) = delete;
private:
	struct Item{
		std::variant<int32_t,float,Vector3> value;
		std::variant<int32_t *,float *,Vector3 *> valuePtr;
	};
	using Group = std::map<std::string,Item>;

	std::map<std::string,Group> datas_;
public:
	template<typename T>
	void setValue(const std::string &groupName,const std::string &itemName,T &value){
		Group &group = datas_[groupName];
		Item newItem = {value,&value};
		group[itemName] = newItem;
	}

	template<typename T>
	bool addValue(const std::string &groupName,const std::string &itemName,T &value){
		auto &group = datas_[groupName];
		auto itemItr = group.find(itemName);
		if(itemItr != group.end()){
			value = std::get<T>(itemItr->second.value);
			return false;
		}
		setValue(groupName,itemName,value);
		return true;
	}

	template<typename T>
	T getValue(const std::string &groupName,const std::string &itemName) const{
		// groupNameの存在を確認
		auto groupItr = datas_.find(groupName);
		assert(groupItr != datas_.end());

		// itemNameの存在を確認
		auto itemItr = groupItr->second.find(itemName);
		assert(itemItr != groupItr->second.end());

		// 指定された型で値を取得
		try{
			return std::get<T>(itemItr->second.value);
		} catch(const std::bad_variant_access &){
			throw std::runtime_error("Incorrect type requested");
		}
	}
};