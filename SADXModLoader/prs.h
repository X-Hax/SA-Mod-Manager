/*
 * https://github.com/essen/prsutil
 * 
 * Archive of the original code from fuzziqer. No license was provided
 * with the released files, although fuzziqer stated using the source
 * freely was fine as long as he's credited for the compression and
 * decompression code.
 * 
 * http://fuzziqer.10.forumer.com/viewtopic.php?t=110
 *
 */

#pragma once

#include <cstdint>

////////////////////////////////////////////////////////////////////////////////

struct PRS_COMPRESSOR
{
	uint8_t bitpos;
	//uint8_t controlbyte;
	uint8_t* controlbyteptr;
	uint8_t* srcptr_orig;
	uint8_t* dstptr_orig;
	uint8_t* srcptr;
	uint8_t* dstptr;
};

void prs_put_control_bit(PRS_COMPRESSOR* pc, uint8_t bit);

void prs_put_control_bit_nosave(PRS_COMPRESSOR* pc, uint8_t bit);

void prs_put_control_save(PRS_COMPRESSOR* pc);

void prs_put_static_data(PRS_COMPRESSOR* pc, uint8_t data);

uint8_t prs_get_static_data(PRS_COMPRESSOR* pc);

////////////////////////////////////////////////////////////////////////////////

void prs_init(PRS_COMPRESSOR* pc, void* src, void* dst);

void prs_finish(PRS_COMPRESSOR* pc);

void prs_rawbyte(PRS_COMPRESSOR* pc);

void prs_shortcopy(PRS_COMPRESSOR* pc, int offset, uint8_t size);

void prs_longcopy(PRS_COMPRESSOR* pc, int offset, uint8_t size);

void prs_copy(PRS_COMPRESSOR* pc, int offset, uint8_t size);

////////////////////////////////////////////////////////////////////////////////

uint32_t prs_compress(void* source, void* dest, uint32_t size);

////////////////////////////////////////////////////////////////////////////////

uint32_t prs_decompress(void* source, void* dest); // 800F7CB0 through 800F7DE4 in mem 

uint32_t prs_decompress_size(void* source);
