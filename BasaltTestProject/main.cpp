﻿#pragma once
#include "request_library.h"

#include <iostream>

using json = nlohmann::json;

int main(int argc, char* argv[])
{
	constexpr size_t parameters_size = 3;
	if (argc != parameters_size)
	{
		std::cout << "Incorrect size of parameters" << std::endl;
		return 1;
	}

	request_library::Request first_request = request_library::get_request(argv[0]);
	request_library::Request second_request = request_library::get_request(argv[1]);

	if (first_request.packages.empty() || second_request.packages.empty())
	{
		std::cout << "Incorrect request " << std::endl;
		return 2;
	}

	std::unordered_map<std::string, std::vector<request_library::Package>> elements_missing_in_the_second_responce =
		request_library::find_missing_elements(first_request, second_request);
	std::unordered_map<std::string, std::vector<request_library::Package>> elements_missing_in_the_first_responce =
		request_library::find_missing_elements(second_request, first_request);
	std::unordered_map<std::string, std::vector<request_library::Package>> release_elements =
		request_library::find_released_elements(first_request, second_request);

	request_library::CompareBranchesStructure structure = generate_structure("p10", "sisyphus", elements_missing_in_the_first_responce,
		elements_missing_in_the_second_responce, release_elements);

	request_library::write_to_file(argv[2], structure);

	return 0;
}
