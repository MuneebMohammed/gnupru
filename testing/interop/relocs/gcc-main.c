
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#include "shared.h"
#include "test.h"

static volatile void * volatile ti_relocs[3] = { &ti_u32, &ti_u16, &ti_u8 };

uint32_t gcc_u32 = 0xccbbeedd;
uint16_t gcc_u16 = 0x4532;
uint8_t gcc_u8 = 0x25;

uint32_t gcc_array16[1024];
uint32_t gcc_array32[1024];

uint64_t gcc_func_arg1(uint32_t arg1)
{
	return arg1 | ((uint64_t)arg1 + 1) << 32;
}

uint32_t gcc_func_arg3(uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
	return arg1 + (arg2 >> 1) + (arg3 >> 2);
}

uint32_t gcc_func_arg4(uint32_t n, uint32_t arg1, uint32_t arg2,
		       uint32_t arg3, uint32_t arg4)
{
        uint32_t s = 0;

        while (n--) {
                uint64_t tmp = gcc_func_arg1(arg1);

                s += tmp & 0xffffffff;
                s += tmp >> 32;
                s += gcc_func_arg3(arg2, arg3, arg4);
        }

        return s;
}


int main(void)
{
	const uint32_t ARG1 = 0x54321;
	const uint32_t ARG2 = 0x12345678;
	const uint32_t ARG3 = 0xaabbccdd;
	const uint32_t ARG4 = 0x0;

	uint32_t ti_result, gcc_result;

	ti_result = ti_func_arg4(10, ARG1, ARG2, ARG3, ARG4);
	gcc_result = ti_func_arg4(10, ARG1, ARG2, ARG3, ARG4);

	printf("TI: 0x%08"PRIx32", GCC: 0x%08"PRIx32"\n", ti_result, gcc_result);

	CHECK (gcc_u32 == *(uint32_t*)ti_relocs[0]);
	CHECK (gcc_u16 == *(uint16_t*)ti_relocs[1]);
	CHECK (gcc_u8 == *(uint8_t*)ti_relocs[2]);
	CHECK (ti_result == gcc_result);
	CHECK (ti_ptr16 == &gcc_array16[678]);
	CHECK (ti_ptr32 == &gcc_array32[123]);

	printf("SUCCESS\n");
	return EXIT_SUCCESS;
}
