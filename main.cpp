//include "App.h"
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#ifdef __SDSCC__
#include <ff.h>
#include <sds_lib.h>
#endif

//#define FRAME_SIZE (960 * 540)
//#define FRAMES (10)
//#define STAGES (5)
//#define MAX_OUTPUT_SIZE 15000
unsigned char * Data;

void Exit_with_error(void)
{
  perror(NULL);
  exit(EXIT_FAILURE);
}

#ifdef __SDSCC__
void Check_error(int Error, const char * Message)
{
  if (Error)
  {
    puts(Message);
    exit(EXIT_FAILURE);
  }
}
#endif

void Load_data(char Data[15000])
{
//int Size = 10;
int Size = 14322;

#ifdef __SDSCC__
  FIL File;
  unsigned int Bytes_read;

  FRESULT Result = f_open(&File, "pfile.txt", FA_READ);
  Check_error(Result != FR_OK, "Could not open input file.");

  Result = f_read(&File, Data, Size, &Bytes_read);
  Check_error(Result != FR_OK || Bytes_read != Size, "Could not read input file.");

  Check_error(f_close(&File) != FR_OK, "Could not close input file.");
#else
  FILE * File = fopen("p_file.txt", "rb");
  if (File == NULL)
    Exit_with_error();

  if (fread(Data, 1, Size, File) != Size)
    Exit_with_error();

  if (fclose(File) != 0)
    Exit_with_error();
#endif
}

void Store_data(const char * Filename, char Data[15000], int Size)
{
#ifdef __SDSCC__
  FIL File;
  unsigned int Bytes_read;

  FRESULT Result = f_open(&File, Filename, FA_WRITE | FA_CREATE_ALWAYS);
  Check_error(Result != FR_OK, "Could not open output file.");

  Result = f_write(&File, Data, Size, &Bytes_read);
  Check_error(Result != FR_OK || Bytes_read != Size, "Could not write output file.");

  Check_error(f_close(&File) != FR_OK, "Could not close output file.");
#else
  FILE * File = fopen(Filename, "wb");
  if (File == NULL)
    Exit_with_error();

  if (fwrite(Data, 1, Size, File) != Size)
    Exit_with_error();

  if (fclose(File) != 0)
    Exit_with_error();
#endif
}



int main()
{
//  char input_array [300000000];
//  char output_array [300000000];
  char input_array [15000];
  char output_array [15000];
    //char * Input_data;
    //char * Output_data;

#ifdef __SDSCC__
  FATFS FS;
  // Mount SD-card.
  Check_error(f_mount(&FS, "0:/", 0) != FR_OK, "Could not mount SD-card");
#endif


    
//    printf("before load");
//int lSize = 200273920;
  int lSize = 14322;
  Load_data(input_array);
//  input_array[lSize] = '\0';
//printf("input %s \n", input_array);


  unsigned long long cdc_Start = 0;
  unsigned long long cdc_Duration = 0;
  unsigned long long sha_Start = 0;
  unsigned long long sha_Duration = 0;
  unsigned long long lzw_Start = 0;
  unsigned long long lzw_Duration = 0;
  unsigned long long dup_Start = 0;
  unsigned long long dup_Duration = 0;
  //chunk chunk1;
  char*input;
  input = input_array;
  char chunk[TABLE_SIZE] = { 0 };
  char* chunk_buffer = chunk;
  int chunk_cal_table[2] = { 0 };
  int* chunk_cal = chunk_cal_table;
  int chunk_size = 0;

  uint32_t hash_table[8] = { 0 };
  uint32_t* hash = hash_table;


  int LZW_index_table[MAX_CHUNK_NUMBER];
  int* LZW_index = LZW_index_table;
  int dup_index_table[MAX_CHUNK_NUMBER];
  int* dup_index = dup_index_table;


  LZW_index[0] = 0;
  for (int m = 1; m < MAX_CHUNK_NUMBER; m++) {
	  LZW_index[m] = -1;
  }

  uint32_t header = 0;
  int lzw_chunkSize = 0;

  char str1[4]; //header
  char str[] = "HEAD";
  int k = 0;
  int dup = 0;
  int single_chunkSize = 0;
  int lzw_count = 0;
  int start_position = 0;
  int end_position = 0;
  int output_array_count = 0;

  //#ifdef __SDSCC__

    unsigned long long Start = sds_clock_counter();
  //#endif

    int times = 0;
  while (1) {
      
	  times++;
      cdc_Start = sds_clock_counter();
	  input = cdc(input, chunk_buffer, chunk_cal);
      cdc_Duration = sds_clock_counter() - cdc_Start + cdc_Duration;

	  chunk_size = chunk_cal[0];
//	  printf("chunk size%d\n", chunk_size);

      sha_Start = sds_clock_counter();
	  SHA(chunk_buffer, chunk_size, hash);
      sha_Duration = sds_clock_counter() - sha_Start + sha_Duration;


      dup_Start =sds_clock_counter();
	  dedup(hash, LZW_index, dup_index);
      dup_Duration = sds_clock_counter() - dup_Start + dup_Duration;
	  //printf("\n");

      lzw_Start =sds_clock_counter();
      output_array_count = lzwcompression(chunk_buffer, output_array, chunk_size, output_array_count, LZW_index, dup_index);
   	  lzw_Duration = sds_clock_counter() - lzw_Start + lzw_Duration;

	  if (chunk_cal[1] == 1) break;

  }

  unsigned long long Duration = sds_clock_counter()- Start;

  printf("times: %d.\n", times);
  printf("Duration: %llu cycles.\n", Duration);
  printf("lzw_Duration: %llu cycles.\n", lzw_Duration);
  printf("cdc_Duration: %llu cycles.\n", cdc_Duration);
  printf("sha_Duration: %llu cycles.\n", sha_Duration);
  printf("dup_Duration: %llu cycles.\n", dup_Duration);


 Store_data("LP_out.txt", output_array, output_array_count);
//  fwrite(output_array, 1, output_array_count, Output_file);
  //free(Input_data);
  //free(Output_data);
  printf("total compressed chunks' size is: %d byte\n", output_array_count);
  puts("Application completed successfully.");
  //getchar();
  return 0;
}
