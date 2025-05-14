#pragma once

#ifndef TEST
#define TEST
#endif
#include <string>

class Parent
{
	
};

// This is a test sample class"
static float var = 5.f;
template<typename T> class Class final : public Parent {
public:
	void fn_test(T value) {}
private:
	std::string internal_name = "InternalName";
};