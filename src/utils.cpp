/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 14:00:20 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/04 14:37:47 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::vector<std::string> split(const std::string& str, char delim, bool acceptEmptyLines)
{
	std::vector<std::string> split;
	
	size_t start = 0;
	while (start != std::string::npos)
	{
		size_t end = str.find(delim, start);
		std::string result = str.substr(start, end - start);

		if (result.length() || acceptEmptyLines)
			split.push_back(result);

		start = (end != std::string::npos) ? end + 1 : std::string::npos;
	}

	return split;
}
