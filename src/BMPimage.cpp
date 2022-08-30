/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BMPimage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 15:06:04 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/29 17:00:43 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdexcept>
#include <fstream>
#include <iostream>
#include "BMPimage.hpp"

BMPimage::BMPimage(const std::string& filename)
{
	std::ifstream ifs(filename, std::ios::binary);
	if (ifs.fail())
		throw std::runtime_error(filename + ": failed to open image");

	ifs.read(reinterpret_cast<char*>(&file), sizeof(FileHeader));
	if (file.type != 0x4D42) // should be "BM"
		throw std::invalid_argument(filename + ": incorrect file type");

	ifs.read(reinterpret_cast<char*>(&info), sizeof(InfoHeader));

	if (info.size != 40 && info.size != 108 && info.size != 124)
		throw std::invalid_argument(filename + ": incorrect DIB format");
	if (info.bpp != 24 && info.bpp != 32)
		throw std::invalid_argument(filename + ": only 24 and 32 bits images are supported");
	if (info.compression != 0 && info.compression != 3)
		throw std::invalid_argument(filename + ": data compression is not supported");
	_M_IsReversed = (info.height < 0);
	info.height = std::abs(info.height);

	ifs.seekg(file.offset, ifs.beg);
	_M_Pixels.resize(info.width * info.height * 4, 0xFF);

	const uint8_t bytesPerPixel = info.bpp / 8;
	const size_t stride = info.width * bytesPerPixel;
	const uint8_t padding = stride % 4;

	for (int y = 0; y < info.height; y++)
	{
		for (int x = 0; x < info.width; x++)
		{
			size_t index;
			if (_M_IsReversed)
				index = (y * info.width + x) * 4;
			else
				index = ((info.height - (y + 1)) * info.width + x) * 4;
			ifs.read(reinterpret_cast<char*>(_M_Pixels.data() + index), bytesPerPixel);
		}
		ifs.seekg(padding, ifs.cur);
	}
	ifs.close();
}

const uint8_t*
BMPimage::data() const
{
	return _M_Pixels.data();
}
