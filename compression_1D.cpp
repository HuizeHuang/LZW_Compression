
#include "header.h"
/*data structure for dictionary*/
//typedef struct prex_cha {
//	int value; /*index for dictionary*/
//	unsigned int prefix;
//	unsigned int character; /*current character*/
//}Hash_Table;

//Hash_Table Hash_table[TABLE_SIZE];

void initial_hashtable(int hash_value_lzw[TABLE_SIZE])
{
	int i;
	for (i = 0; i < TABLE_SIZE; i++)
	{
//#pragma HLS unroll factor=1024
#pragma HLS pipeline II=1
//#pragma HLS array_partition variable=Hash_table dim=1
//		Hash_table[i].value = not_used;
		hash_value_lzw[i] = not_used;
	}
}

/*Output every 8 bit*/
int output_lzw(code_type code, char output_array[50000], int end, int output_array_count)
{


	static int count = 0;
	static unsigned long buffer = 0L; /*buffer */


	buffer |= (unsigned long)code << (32 - bitsize - count);
	count += bitsize;
	while (count >= 8)
	{
		output_array[output_array_count] = buffer >> 24;

		//printf("output_array %d\n", code);
		output_array_count++;
		//fputc(buffer >> 24, out);
		//lzw_chunkSize++;
		buffer <<= 8;
		if (end) {
			count = 0;
			//output_array_count--;
		}
		else
			count -= 8;

	}
	return output_array_count;
    
   
}

//create a hash table and find the match p+c
int find_match(int prefix, int character,int hash_value_lzw[TABLE_SIZE], int hash_prefix[TABLE_SIZE], int hash_character[TABLE_SIZE]){
    
//	int index;
//	index = prefix % TABLE_SIZE;
//	while (1)
//	{
//		if (hash_value_lzw[index] == not_used)
//		{
//			return (index);
//		}
//		if (hash_prefix[index] == prefix && hash_character[index] == character)
//		{
//			return (index);
//		}
//		index = index + 1;
//		if (index >= TABLE_SIZE)
//		{
//			index = index - TABLE_SIZE;
//		}
//	}
	int index;
	 int offset;                          /*offset为偏移位*/
	 int i;
	 index = (character << 5) ^ prefix;  /*用异或来决定index*/
	if (index == 0)
	 {
	  offset = 1;
	 }
	 else
	 {
	  offset = 8192 - index;
	 }

//	 while (1)
//	 {
//	  if (hash_value_lzw[index] == not_used)
//	  {
//	   return (index);
//	  }
//	  if (hash_prefix[index] == prefix && hash_character[index] == character)
//	  {
//	   return (index);
//	  }
//	  index -= offset;
//	  if (index < 0)
//	  {
//	   index += 8192;
//	  }
//	 }
	 for(i = 0; i < TABLE_SIZE; i++)
	 {
#pragma HLS unroll factor=1024
#pragma HLS pipeline
	  if (hash_value_lzw[index] == not_used)
	  {
	   return (index);
	   break;
	  }
	  if (hash_prefix[index] == prefix && hash_character[index] == character)
	  {
	   return (index);
	   break;
	  }
	  index -= offset;
	  if (index < 0)
	  {
	   index += 8192;
	  }
	 }
}



int lzwcompression(char chunk[TABLE_SIZE], char output_array[50000], int chunk_size, int output_array_count,int LZW_index[MAX_CHUNK_NUMBER], int dup_index[MAX_CHUNK_NUMBER])
{

	int prefix;
	int character;
	int index;
	int next_code = 256;
    int hash_value_lzw[TABLE_SIZE];
    int hash_prefix[TABLE_SIZE];
    int hash_character[TABLE_SIZE];
    
    uint32_t header = 0;
    int lzw_chunkSize = 0;

    char str1[4]; //header
    char str[] = "HEAD";
    int k = 0;
    int dup = 0;
    int single_chunkSize = 0;
    static int lzw_count = 0;
    int start_position = 0;
    int end_position = 0;


	initial_hashtable(hash_value_lzw);
    //memset(hash_value_lzw,-1,TABLE_SIZE);
    //std::fill_n(hash_value_lzw,TABLE_SIZE,-1);
	int i = 1;
	prefix = int(chunk[0]);

	  if (LZW_index[lzw_count++] == -1) {            //if it's a duplicated chunk
			  header = dup_index[k] << 1;
			  k++;
			  header = header | 1;
			  //printf("header:%d\n", header);
			  str1[0] = (char)(header & 0xff);
			  str1[1] = (char)((header >> 8) & 0xff);
			  str1[2] = (char)((header >> 16) & 0xff);
			  str1[3] = (char)((header >> 24) & 0xff);

			  //fseek(output_file, 0, SEEK_END);
			  for (int p = 0; p < 4; p++) {
				  output_array[output_array_count++] = str1[p];
				  //fputc(str1[p], output_file);
			  }

		  }
		  else {  /************************ LZW Compression ************************/
			  //int start_position = ftell(output_file); //record current position
			  start_position = output_array_count;

			  //printf("start_position:%d\n", start_position);

			  output_array_count += 4;

	          for (int k = 0; k < chunk_size - 1; k++) {
	          //#pragma HLS unroll factor=1024
	          //#pragma HLS pipeline
	          		character = chunk[i++];
	          		index = find_match(prefix, character, hash_value_lzw,hash_prefix,hash_character);

	                  if (next_code == TABLE_SIZE){
	                      next_code = next_code &( TABLE_SIZE-1);
	                  }
	          		if (hash_value_lzw[index] != not_used)/*if p has shown up*/
	          		{
	          			prefix = hash_value_lzw[index];
	          		}
	          		else
	          		{
	          			if (next_code <= TABLE_SIZE)/*if p hasn't shown up*/
	          			{
	          				hash_value_lzw[index] = next_code++;
	          				hash_character[index] = character;
	          				hash_prefix[index] = prefix;
	          			}

	          			output_array_count = output_lzw(prefix, output_array, 0, output_array_count);

	          			prefix = character; /*p = c*/

	          		}/*ifelse*/
	          	}/*while*/
	          	output_array_count = output_lzw(prefix, output_array, 0, output_array_count);
	          	output_array_count = output_lzw(0, output_array, 1, output_array_count);
			  //single_chunkSize = lzw_chunkSize - single_chunkSize;

	      	  if (output_array[output_array_count - 1] == 0) {
	      			  output_array_count--;
	      		  }
			  //int end_position = ftell(output_file);
			  end_position = output_array_count;
			  //printf("end_position:%d\n", end_position);

			  single_chunkSize = end_position - start_position - 4;
			  header = single_chunkSize << 1;
			  //printf("single compressed chunk's size is: %d byte\n", single_chunkSize);
			  //transform header which is 32 bit int to character to store in the file
			  str1[0] = (char)(header & 0xff);
			  str1[1] = (char)((header >> 8) & 0xff);
			  str1[2] = (char)((header >> 16) & 0xff);
			  str1[3] = (char)((header >> 24) & 0xff);

			  //fseek(output_file, start_position, SEEK_SET); //move pointer to the head of file

			  for (int i = 0; i < 4; i++) {
				  output_array[start_position] = str1[i];
				  start_position++;
				  //fputc(str1[i], output_file);
			  }
			  //fseek(output_file, 0, SEEK_END);
			  //printf("header:%d\n", header);
		  }
	return output_array_count;

}
