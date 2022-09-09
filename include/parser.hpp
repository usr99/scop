/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 19:41:45 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/08 20:04:24 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _PARSER_HPP_
# define _PARSER_HPP_

# include <map>
# include <vector>
# include <string>

# include "Material.hpp"

using MaterialLibrary = std::map<std::string, Material>;
using LineElements = std::vector<std::string>;

void checkFileExtension(const std::string& filename, const std::string& extension);
LineElements split(const std::string& line, char delimiter = ' ', bool allowEmptyLines = false);
void loadMaterialLibrary(const std::string& filename, MaterialLibrary& lib);

std::string parseString1(const LineElements& elements);
unsigned int parseUInt1(const LineElements& elements);
float parseFloat1(const LineElements& elements);
ft::vec3 parseFloat3(const LineElements& elements);

#endif
