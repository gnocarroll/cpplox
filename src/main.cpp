#include <string_view>
#include <vector>

#include "Lox.hpp"

int main(int argc, char **argv) {
	std::vector<std::string_view> args(argc);

	for (int i = 0; i < argc; i++) args[i] = argv[i];

	return Lox::loxMain(args);
}