/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderProgram.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 23:36:27 by mamartin          #+#    #+#             */
/*   Updated: 2022/09/06 18:41:13 by mamartin         ###   ########.fr       */
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
	
		ShaderProgram(const std::string& filepath, bool bindAfterConstruction = true);
		~ShaderProgram();

		void bind() const;
		void unbind() const;

		void setUniform1i(const std::string& name, int value);
		void setUniform1iv(const std::string& name, unsigned int count, int* data);
		void setUniform1ui(const std::string& name, unsigned int value);
		void setUniform1f(const std::string& name, float value);
		void setUniform3f(const std::string& name, float v0, float v1, float v2);
		void setUniformVec3f(const std::string& name, const glm::vec3& value);
		void setUniformVec3f(const std::string& name, const vec3& value);
		void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
		void setUniformBlock(const std::string& name, unsigned int binding);

	private:

		std::string		_parseShader(const std::string &filepath);
		unsigned int	_compileShader(unsigned int type, const std::string &source);
		unsigned int	_createShader(const std::string &vertexShader, const std::string &fragmentShader);
		int				_getUniformLocation(const std::string& name);
		int				_getUniformBlockIndex(const std::string& name);

		unsigned int							_id;
		std::string								_filepath;
		std::unordered_map<std::string, int>	_uniformLocationCache;
};

#endif
