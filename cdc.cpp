#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"

//typedef struct {
//	char*input;
//	int chunk_size;
//	char chunk_buffer[MAX_CHUNK_NUMBER];
//	int check_end;
//}chunk;

//#pragma SDS data access_pattern(input:SEQUENTIAL, chunk_buffer:SEQUENTIAL)
//#pragma SDS data mem_attribute(input:PHYSICAl_CONTIGUOUS, chunk_buffer:PHYSICAL_CONTIGUOUS)

char* cdc(char* input, char chunk_buffer[TABLE_SIZE], int chunk_cal[2])
//void cdc(int file_length, unsigned char file_buffer[1514], int chunk_size[50], unsigned char chunk[50][8192])
{
//#pragma HLS inline region
//	chunk chunk1;
	int j = 0;
	int n = 0;
	int a = 10;
	int an = 1;
	int mod = 4096;//last 12 bit
	int max = 8192;//maximum 8KB
	uint64_t hash_window[WINDOW_WIDTH];
	uint64_t hash_value = 0;
	char c;
	//c = input_array[count];

	while(1) {
		//Initialization
		if (n < (WINDOW_WIDTH)) {
			c= *input;
			//c = *input;
			chunk_buffer[n] = c;
			if (c == '\0') {
				chunk_cal[0] = n;
				chunk_cal[1] = 1;
				break;
			}
			hash_value = (hash_value * a + c) % mod;
			hash_window[n] = hash_value;
			an = (an*a) % mod;
			n++;
			input++;
		}
		//calculate hash
		else if (hash_value != 0 && n < max) {
			//c = file_buffer[i];
			c=*input;
			chunk_buffer[n] = c;
			if (c == '\0') {
				chunk_cal[0] = n;
				chunk_cal[1] = 1;
				break;
			}
			hash_value = (hash_value * a + c - an * hash_window[0]) % mod;
			for (j = 0; j < WINDOW_WIDTH - 1; j++) {
				hash_window[j] = hash_window[j + 1];
			}
			hash_window[WINDOW_WIDTH - 1] = hash_value;
			n++;
			input++;
		}
		//cut_chunk
		else {
			chunk_cal[0] = n;
			//input_array++;
			break;
		}
	}
	return input;
	//chunk_size[i] = n;
}

//void get_chunk(FILE * pfile, int chunk_num, int chunk_size_table[50], unsigned char chunk[50][8192]) {
//	int i = 0;
//	int j = 0;
//	int k = 0;
//	int end_position_table[50] = { 0 };
//	unsigned int c;
//	for (i = 0; i < chunk_num; i++) {
//		if (i == 0) end_position_table[0] = chunk_size_table[0] + 1;
//		else end_position_table[i] = end_position_table[i - 1] + chunk_size_table[i] + 1;
//	}
//
//	i = 0;
//	for (j = 0; j < chunk_num; j++) {
//		k = 0;
//		while (i < end_position_table[j]) {
//			//if ((i >= start_position[j]) && (i <= (start_position[j] + chunk_size[chunk_num]))) {
//			c = fgetc(pfile);
//			chunk[j][k] = c;
//			//printf("%c", c);
//			k++;
//			i++;
//			//c = fgetc(pfile);
//		}
//		//printf("\n end \n");
//
//	}
//
//}
