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
	using Item = std::variant<int32_t,float,Vector3>;
	using Group = std::map <std::string,Item>;

	std::map<std::string,Group> datas_;
public:
	void setValue(const std::string &groupName,const std::string &itemName,int32_t value);
	void setValue(const std::string &groupName,const std::string &itemName,float value);
	void setValue(const std::string &groupName,const std::string &itemName,Vector3 value);

	bool addValue(const std::string &groupName,const std::string &itemName,int32_t value);
	bool addValue(const std::string &groupName,const std::string &itemName,float value);
	bool addValue(const std::string &groupName,const std::string &itemName,Vector3 value);

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
			return std::get<T>(itemItr->second);
		} catch(const std::bad_variant_access &){
			throw std::runtime_error("Incorrect type requested");
		}
	}
};