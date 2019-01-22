//#ifndef SRC_APP_H_
#define SRC_APP_H_

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

#define INT_BITS 32
#define WINDOW_WIDTH 4
#define hash_value_size 8
#define MAX_CHUNK_NUMBER 20

#define TABLE_SIZE   8192
#define not_used  -1
#define MAXVAL(n)  (( 1 <<( n )) -1)
#define CODE_LENGTH  13
#define bitsize 13



//#define hash_value_lzw         0
//#define hash_prefix         1
//#define hash_character         2
typedef uint16_t    code_type;



typedef struct {
	char*input;
	int chunk_size;
	char chunk_buffer[MAX_CHUNK_NUMBER];
	int check_end;
}chunk;

char* cdc(char* input, char chunk_buffer[TABLE_SIZE], int chunk_cal[2]);
//char* cdc(char chunk_buffer[TABLE_SIZE]);
//void cdc(int file_length, unsigned char file_buffer[1514], int chunk_size[50], unsigned char chunk[50][8192]);
//void cdc(char input_array[15000], int chunk_size[MAX_CHUNK_NUMBER], char chunk[MAX_CHUNK_NUMBER][TABLE_SIZE]);
//void get_chunk(FILE * pfile, int chunk_num, int chunk_size_table[50], unsigned char chunk[50][8192]);
void SHA(char chunk[TABLE_SIZE], int chunk_size, uint32_t hash[8]);
//void SHA(char chunk[MAX_CHUNK_NUMBER][TABLE_SIZE], int chunk_num, int chunk_size_table[MAX_CHUNK_NUMBER], uint32_t hash[MAX_CHUNK_NUMBER][8]);
//void deduplication(char output_array[15000], int chunk_num, uint32_t hash[MAX_CHUNK_NUMBER][8], int LZW_index[MAX_CHUNK_NUMBER], int dup_index[MAX_CHUNK_NUMBER]);
void dedup(uint32_t hash[8], int LZW_index[MAX_CHUNK_NUMBER], int dup_index[MAX_CHUNK_NUMBER]);
//void lzwcompression(unsigned char chunk_table[MAX_CHUNK_NUMBER][8192], unsigned char output_array[15000], int chunk_index, int chunk_size_table[MAX_CHUNK_NUMBER]);
//int compression(char Input_data[15000], char Output_data[15000]);
//void compression(int file_length, unsigned char file_buffer[1514]);
int lzwcompression(char chunk[TABLE_SIZE], char output_array[15000], int chunk_size, int output_array_count, int LZW_index[MAX_CHUNK_NUMBER], int dup_index[MAX_CHUNK_NUMBER]);


//#endif

