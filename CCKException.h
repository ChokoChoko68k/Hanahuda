/*
cck68k Original Exception Class 
*/
#pragma once
#include<string>

class CCKException
{
	//variables
	//inirializer
	std::string errormes{ "error" };

	//functions
	void SetErrorMes(std::string mes) { errormes = mes; }
	std::string GetErrorMes() { return errormes; }

	//con&des
	CCKException() {};
	CCKException(const CCKException& other) {
		errormes = *(new std::string(other.errormes));
	};
	virtual ~CCKException() {
		delete &errormes;
	};
};
