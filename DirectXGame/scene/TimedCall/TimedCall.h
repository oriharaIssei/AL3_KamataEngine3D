#pragma once

#include <functional>
#include <memory>

#include <stdint.h>

template<typename Return, typename... args> class TimedCall {
public:
	TimedCall(std::function<Return(args...)> function, int32_t time) : function_(function), time_(time){};

	void Update();

private:
	std::function<Return(args...)> function_;

	int32_t time_;
	bool isOccured_ = false;

public:
	bool getIsOccured() const { return isOccured_; }
};
template<typename Return, typename... args> inline void TimedCall<Return, args...>::Update() {
	if (isOccured_) {
		return;
	}

	if (--time_ <= 0) {
		isOccured_ = true;
		function_();
	}
}
