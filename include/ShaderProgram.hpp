/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderProgram.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 23:36:27 by mamartin          #+#    #+#             */
/*   Updated: 2022/07/12 19:43:13 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SHADER_PROGRAM_HPP_
# define _SHADER_PROGRAM_HPP_

# include <string>
# include <unordered_map>

# include "math.hpp"

class ShaderProgram
{	
	public:
	
		ShaderProgram(const std::string& filepath);
		~ShaderProgram();

		void bind() const;
		void unbind() const;

		void setUniform1i(const std::string& name, int value);
		void setUniform3f(const std::string& name, float v0, float v1, float v2);
		void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

	private:

		std::string		_parseShader(const std::string &filepath);
		unsigned int	_compileShader(unsigned int type, const std::string &source);
		unsigned int	_createShader(const std::string &vertexShader, const std::string &fragmentShader);
		int				_getUniformLocation(const std::string& name);

		unsigned int							_id;
		std::string								_filepath;
		std::unordered_map<std::string, int>	_uniformLocationCache;
};

#endif
