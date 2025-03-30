#pragma once

#include <optional>
#include <string_view>

#define INJECT_OBJ_TYPES \
	LOX_OBJ(NIL) \
	LOX_OBJ(BOOLEAN) \
	LOX_OBJ(NUMBER) \
	LOX_OBJ(STRING) \
	LOX_OBJ(LOX_OBJ_TYPE_COUNT) \
	LOX_OBJ(INVALID)

class LoxObject {
	static constexpr char* cStrs[] = {
#define LOX_OBJ(name) (char * const) #name,

		INJECT_OBJ_TYPES

#undef LOX_OBJ

		(char* const) ""
	};

public:
	enum Type {
#define LOX_OBJ(name) name,

		INJECT_OBJ_TYPES

#undef LOX_OBJ
	};

	Type type = INVALID;

	struct {
		bool boolean = false;
		double number = 0.0;
		std::string_view stringView = "";
	} data;

	LoxObject(Type type) : type(type) {
		if (type < 0 || type >= LOX_OBJ_TYPE_COUNT) type = INVALID;
	}

	LoxObject(bool boolean) {
		type = BOOLEAN;
		data.boolean = boolean;
	}
	LoxObject(double number) {
		type = NUMBER;
		data.number = number;
	}
	LoxObject(std::string_view sv) {
		type = STRING;
		data.stringView = sv;
	}

	LoxObject() {}

	LoxObject(LoxObject& other) = delete;
	LoxObject(LoxObject&& other) = default;

	LoxObject& operator=(LoxObject& other) = delete;
	LoxObject& operator=(LoxObject&& other) = default;

	bool isValidType() {
		return type != INVALID;
	}

	std::optional<bool> getBoolean() const {
		if (type != BOOLEAN) return {};

		return data.boolean;
	}
	std::optional<double> getNumber() const {
		if (type != NUMBER) return {};

		return data.number;
	}
	std::optional<std::string_view> getString() const {
		if (type != STRING) return {};

		return data.stringView;
	}

	std::string toString() {
		switch (type) {
		case NIL: return "nil";
		case BOOLEAN: return data.boolean ? "true" : "false";
		case NUMBER: return std::to_string(data.number);
		case STRING: return std::string(
			data.stringView.data(),
			data.stringView.size());
		default:
			break;
		}

		return "invalid";
	}
};

#undef INJECT_OBJ_TYPES