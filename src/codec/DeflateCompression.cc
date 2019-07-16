/*-
 This file is part of AFF4 CPP.

 AFF4 CPP is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 AFF4 CPP is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with AFF4 CPP.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DeflateCompression.h"
#include "zlib.h"

namespace aff4 {
namespace codec {

DeflateCompression::DeflateCompression(uint32_t chunkSize) :
		CompressionCodec(aff4::lexicon::getLexiconString(aff4::Lexicon::AFF4_IMAGE_COMPRESSION_DEFLATE), chunkSize) {
}

DeflateCompression::~DeflateCompression() {
	// NOP
}

uint64_t DeflateCompression::decompress(void* source, uint64_t srcSize, void* destination, uint64_t destSize) NOEXCEPT {
	if (source == nullptr || destination == nullptr) {
		return 0;
	}
	// Decompress.
	z_stream zstream;
	::memset(&zstream, 0, sizeof(zstream));
	zstream.next_in = (Bytef*)source;
	zstream.avail_in = (uInt)srcSize;
	zstream.next_out = (Bytef*)destination;
	zstream.avail_out = (uInt)destSize;

	if (inflateInit2(&zstream, -15) != Z_OK) {
		return -1;
	}

	if (inflate(&zstream, Z_FINISH) != Z_STREAM_END) {
		inflateEnd(&zstream);
		return -1;
	}

	inflateEnd(&zstream);
	return zstream.total_out;
}

} /* namespace codec */
} /* namespace aff4 */
