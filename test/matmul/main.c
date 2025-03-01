// SPDX-License-Identifier: MIT
/* Copyright 2022 Eileen Yoon <eyn@gmx.com> */

#include <stdio.h>

#include "ane.h"
#include "ane_f16.h"

// clang-format off

int main(void)
{
	int err = 0;

	float A[] = { 0.11, 0.12, 0.13,
                      0.21, 0.22, 0.23 };
	uint16_t Ah[6];
	ane_f32_to_f16_row(A, Ah, 6);

	float B[] = { 1011, 1012,
	              1021, 1022,
	              1031, 1032 };
	uint16_t Bh[6];
	ane_f32_to_f16_row(B, Bh, 6);

	float C[] = { 0.00, 0.00,
                      0.00, 0.00 };
	float Cf[4];
	uint16_t Ch[4];

	struct ane_nn *nn = ane_init("matmul.anec");
	if (nn == NULL) {
		printf("failed to init\n");
		return -1;
	}

	printf("elem: 0x%lx\n", ane_src_elem(nn, 0));
	printf("elem: 0x%lx\n", ane_src_elem(nn, 1));
	printf("elem: 0x%lx\n", ane_dst_elem(nn, 0));

	ane_tile_send(nn, Ah, 0);
	ane_tile_send(nn, Bh, 1);

	err = ane_exec(nn);

	ane_tile_read(nn, Ch, 0);

	ane_f16_to_f32_row(Ch, Cf, 4);
	printf("[ %g, %g\n", Cf[0], Cf[1]);
	printf("  %g, %g ]\n", Cf[2], Cf[3]);

	ane_free(nn);

	return err;
}

// clang-format on
