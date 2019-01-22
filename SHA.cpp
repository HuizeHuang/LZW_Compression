#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

int rrt(int n, uint32_t d)
{
	/* In n>>d, first d bits are 0. To put last 3 bits of at
	first, do bitwise or of n>>d with n <<(INT_BITS - d) */
	return (n >> d) | (n << (INT_BITS - d));
}

#pragma SDS data zero_copy(chunk[0:TABLE_SIZE], hash[0:8])
void SHA(char chunk[8192], int chunk_size, uint32_t hash[8])
{
	//allocated chunk size is 8 KB

	//Initialize hash values:
	//(first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19):
	//32 bits = 4 bytes = unsigned int

	//uint32_t test = 1;
	//printf("%d, %d\n", test,rrt(7,test));
	//printf("%d, %d\n", test,(test>>7));

	uint32_t h0 = 0x6a09e667;
	uint32_t h1 = 0xbb67ae85;
	uint32_t h2 = 0x3c6ef372;
	uint32_t h3 = 0xa54ff53a;
	uint32_t h4 = 0x510e527f;
	uint32_t h5 = 0x9b05688c;
	uint32_t h6 = 0x1f83d9ab;
	uint32_t h7 = 0x5be0cd19;

	int L, K;
	int i, j, m, n, p;
	uint32_t S1, S0, s1, s0, a, b, c, d, e, f, g, h, temp1, temp2, ch, maj;


	//Initialize array of round constants:
	//(first 32 bits of the fractional parts of the cube rootse of the first 64 primes 2..311):
	uint32_t k[64] =
	{ 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

	//Pre-processing (Padding):
	//begin with the original message of length L bits
	L = chunk_size + 1; //start with 0
						//append a single '1' bit
	chunk[chunk_size] = 0;
	//append K '0' bits, where K is the minimum number >= 0 such that L + 1 + K + 64 is a multiple of 512
	K = L % 16; //each chunk should be multiple of 16
	K = 16 - K;
	for (p = 1; p <= K; p++) {
#pragma HLS unroll
#pragma HLS pipeline
		chunk[L + p] = 0;
	}
	//append L as a 64-bit big-endian integer, making the total post-processed length a multiple of 512 bits = 64 * 8bits

	//Process the message in successive 512-bit chunks:
	//break message into 512-bit chunk
	int message_num = (L + K)/16;

	//each chunk should be multiple of 16
	for (m = 0; m < message_num; m++) {
#pragma HLS unroll
#pragma HLS pipeline
		//create a 64-entry message schedule array w[0..63] of 32-bit words
		//(The initial values in w[0..63] don't matter, so many implementations zero them here)
		unsigned int w[64] = { 0 };

		//copy chunk into first 16 words w[0..15] of the message schedule array
		//64 * 8bits = 32 * 16bits = 512 bits
		for (n = 0; n < 16; n++) {
#pragma HLS unroll
#pragma HLS pipeline
			w[n] = chunk[m * 16 + n];
		}

		//Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array
		for (i = 16; i < 64; i++) {
#pragma HLS unroll
#pragma HLS pipeline
			s0 = (rrt(7, w[i - 15])) ^ (rrt(18, w[i - 15])) ^ (w[i - 15] >> 3);
			s1 = (rrt(17, w[i - 2])) ^ (rrt(19, w[i - 2])) ^ (w[i - 2] >> 10);
			w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		}

		//Initialize working variables to current hash value
		a = h0;
		b = h1;
		c = h2;
		d = h3;
		e = h4;
		f = h5;
		g = h6;
		h = h7;

		//Compression function main loop
		for (j = 0; j <= 63; j++) {
#pragma HLS unroll
#pragma HLS pipeline
//#pragma SDS data access_pattern(hash:SEQUENTIAL)
			S1 = (rrt(6, e)) ^ (rrt(11, e)) ^ (rrt(25, e));
			ch = (e & f) ^ ((!e) & g);
			temp1 = h + S1 + ch + k[i] + w[i];
			S0 = (rrt(2, a)) ^ (rrt(13, a)) ^ (rrt(22, a));
			maj = (a & b) ^ (a & c) ^ (b & c);
			temp2 = S0 + maj;

			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}

		//Add the compressed chunk to the current hash value:
		h0 = h0 + a;
		h1 = h1 + b;
		h2 = h2 + c;
		h3 = h3 + d;
		h4 = h4 + e;
		h5 = h5 + f;
		h6 = h6 + g;
		h7 = h7 + h;

	}
	//Produce the final hash value (big-endian):

	hash[0] = h0;
	hash[1] = h1;
	hash[2] = h2;
	hash[3] = h3;
	hash[4] = h4;
	hash[5] = h5;
	hash[6] = h6;
	hash[7] = h7;
}
