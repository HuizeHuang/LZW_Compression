#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"



void dedup(uint32_t hash[8], int LZW_index[MAX_CHUNK_NUMBER], int dup_index[MAX_CHUNK_NUMBER])
{
	int i = 0;
	int p = 0;
	static int k = 0;
	static int index = 0;
	static int count = 0;
	static uint32_t dedup_table[MAX_CHUNK_NUMBER][8];

	if (index == 0)
	{
		for(int m = 0; m < 8; m++){
			dedup_table[0][m] = hash [m];
		}
		//memcpy(dedup_table[0], hash, 8);
	}
	for (i = 0; i < index; i++) {
		for (p = 0; p < hash_value_size; p++) {
			if (hash[p] != dedup_table[i][p]) {
				break;
			}
		}
		if (p == hash_value_size) {
			dup_index[k] = i;
			k++;
			break;
		}
	}
	if (index == i) {
		for(int m = 0; m < 8; m++){
			dedup_table[index][m] = hash [m];
		}
		//memcpy(dedup_table[index], hash, 32);
		LZW_index[count] = index;
		index++;
	}
	count++;
//    printf("dedup table: \n");
//    for (int j = 0; j < index; j++)
//    {
//        for (int n = 0; n < 8; n++)
//            printf("%u", dedup_table[j][n]);
//        printf("\n");
//    }
//    printf("dedup_index: ");
//    for (int j = 0; j < count; j++)
//    {
//        printf("%d,", dup_index[j]);
//    }
//    printf("\n");
//    printf("LZW_index: ");
//    for (int j = 0; j < count; j++)
//    {
//        printf("%d,", LZW_index[j]);
//    }
//    printf("\n****************\n");
}

//int main()
//{
//    int LZW_index[MAX_CHUNK_NUMBER];
//    int dup_index[MAX_CHUNK_NUMBER];
//    int dup = 0;
//    LZW_index[0] = 0;
//    for (int m = 1; m < MAX_CHUNK_NUMBER; m++) {
//        LZW_index[m] = -1;
//    }
//    uint32_t hash1[] = {1,0,0,0,0,0,0,0};
//    uint32_t hash2[] = {2,0,0,0,0,0,0,0};
//    uint32_t hash3[] = {3,0,0,0,0,0,0,0};
//    uint32_t hash4[] = {1,0,0,0,0,0,0,0};
//    uint32_t hash5[] = {3,0,0,0,0,0,0,0};
//    uint32_t hash6[] = {2,0,0,0,0,0,0,0};
//    uint32_t hash7[] = {4,0,0,0,0,0,0,0};
//    dedup(hash1,LZW_index,dup_index);
//    dedup(hash2,LZW_index,dup_index);
//    dedup(hash3,LZW_index,dup_index);
//    dedup(hash4,LZW_index,dup_index);
//    dedup(hash5,LZW_index,dup_index);
//    dedup(hash6,LZW_index,dup_index);
//    dedup(hash7,LZW_index,dup_index);
//
//    printf("LZW_index:\n");
//    for (int p = 0; p < 7; p++) {
//        printf("%d,", LZW_index[p]);
//    }
//    printf("\n");
//    printf("index of LZW chunk that duplicated chunk refers to:\n ");
//    for (int p = 0; p < 7; p++) {
//        if (LZW_index[p] == -1) {            //if it's a duplicated chunk
//            printf("%d,", dup_index[dup]);
//            dup++;
//        }
//    }
//    printf("\n");
//
//    return 0;
//}

