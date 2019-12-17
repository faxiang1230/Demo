
#include <elf.h>

struct b_info {     // 12-byte header before each compressed block
	uint32_t sz_unc;            // uncompressed_size
	uint32_t sz_cpr;            // compressed_size
	unsigned char b_method;     // compression algorithm
	unsigned char b_ftid;       // filter id
	unsigned char b_cto8;       // filter parameter
	unsigned char b_unused;
};

struct l_info       // 12-byte trailer in header for loader (offset 116)
{
	uint32_t l_checksum;
	uint32_t l_magic;
	uint16_t l_lsize;
	uint8_t  l_version;
	uint8_t  l_format;
};

struct p_info       // 12-byte packed program header follows stub loader
{
	uint32_t p_progid;
	uint32_t p_filesize;
	uint32_t p_blocksize;
};
struct packHeader{
    uint32_t magic;
	uint32_t magic2;
    //uint8_t  version;
    //uint8_t  format;
    //uint8_t  method;
    //uint8_t  level;
    uint32_t u_adler;
    uint32_t c_adler;
    uint32_t u_len;
    uint32_t c_len;
    uint32_t u_file_size;
    uint8_t  filter;
    uint8_t  filter_cto;
    uint8_t  n_mru;
    uint8_t checksum;
};
