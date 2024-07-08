#pragma once

#include <map>

#include <variant>

#include "Vector3.h"
#include <string>

class GlobalVariables {
public:
	static GlobalVariables *getInstance();

	void Update();

	void CreateGroup(const std::string &groupName);

	void SaveFile(const std::string &groupName);

private:
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables &) = delete;
	GlobalVariables *operator=(GlobalVariables &) = delete;
private:
	struct Item {
		std::variant<int32_t,float,Vector3> value;
	};
	struct Group {
		std::map <std::string,Item> items;
	};

	std::map<std::string,Group> datas_;
public:
	void setValue(const std::string &groupName,const std::string &itemName,int32_t value);
	void setValue(const std::string &groupName,const std::string &itemName,float value);
	void setValue(const std::string &groupName,const std::string &itemName,Vector3 value);

};