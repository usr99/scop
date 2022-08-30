/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BMPimage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/28 14:59:56 by mamartin          #+#    #+#             */
/*   Updated: 2022/08/29 17:00:40 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _BMP_IMAGE_HPP_
# define _BMP_IMAGE_HPP_

# include <vector>
# include <cstdint>

struct BMPimage
{
	BMPimage(const std::string& filename);

# pragma pack(push, 1)

	struct FileHeader
	{
		uint16_t type;
		uint32_t size;
		uint32_t reserved;
		uint32_t offset;
	};

	struct InfoHeader
	{
		uint32_t size;
		int32_t  width;
		int32_t  height;
		uint16_t planes;
		uint16_t bpp;
		uint32_t compression;
	};

# pragma pack(pop)

	const uint8_t* data() const;

	FileHeader file;
	InfoHeader info;

	private:

		bool					_M_IsReversed;
		std::vector<uint8_t>	_M_Pixels;
};

#endif
