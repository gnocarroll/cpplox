#include "LoxObject.hpp"

LoxObject LoxObject::nil() {
	LoxObject ret;

	ret.type == LoxObject::NIL;

	return ret;
}

LoxObject LoxObject::boolean() {
	LoxObject ret;

	ret.type == LoxObject::BOOLEAN;

	return ret;
}

LoxObject LoxObject::boolTrue() {
	auto ret = LoxObject::boolean();

	ret.data.boolean = true;

	return ret;
}

LoxObject LoxObject::boolFalse() {
	auto ret = LoxObject::boolean();

	ret.data.boolean = false;

	return ret;
}