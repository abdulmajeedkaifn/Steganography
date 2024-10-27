#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "types.h"
#include "decode.h"
#include "common.h"

void print();

/* Function Definitions */
void red ()
{
  printf("\033[1;31m");
}
void green()
{
    printf("\033[0;32m");
}
void yellow ()
{
  printf("\033[1;33m");
}
void blue ()
{
    printf("\033[0;34m ");
}
void cyan()
{
    printf("\033[0;36m ");
}

/* Function to reset text color to default */

void reset ()
{
  printf("\033[0m");
}


Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{
    /* Storing the image name into the structure element*/
    decInfo->stego_image_fname = argv[2];
    /* Check the image consists of .bmp or not*/
    if(strstr(decInfo->stego_image_fname,".bmp")==NULL)
    {
        red();
        printf("INFO : %s is not a .bmp file\n",decInfo->stego_image_fname);
        reset();
        printf("\n");
        return e_failure;
    }
    
    if (argc > 3) // Ensure argv[3] exists
    {
        /* Storing the file name into the structure element */
        decInfo->output_secret_fname = argv[3];
    }
    else
    {
        /* If argv[3] is not provided, assign the default name */
        green();
        printf("INFO : Default name is decode\n");
        reset();
        printf("\n");
        decInfo->output_secret_fname = "decode";
    }

    return e_success;
}

Status open_files_for_decode(DecodeInfo *decInfo)
{
    decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "r");

    if(decInfo -> fptr_stego_image == NULL)
    {
        red();
        perror("fopen"); // perror will give the reason why file is not opening
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        reset();
        printf("\n");

    	return e_failure;
    }

    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    if(open_files_for_decode(decInfo) == e_failure)
    {
        red();
        printf("INFO : Unable to open files...!\n");
        reset();
        printf("\n");
    }
    else
    {
        print();
        green();
        printf("INFO : Files are opened Successfully...!\n");
        reset();
        printf("\n");
    }


    if(decode_magic_string(decInfo) == e_failure)
    {
        red();
        printf("INFO : magic_string not decoded...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : magic_string decoded successfully...!\n");
        reset();
        printf("\n");
    }

    if(decode_secret_file_extn_size(decInfo) == e_failure)
    {
        red();
        printf("INFO : Secret file extension size not decoded...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : Secret file extension size decoded successfully...!\n");
        reset();
        printf("\n");
    }

    if(decode_secret_file_extn(decInfo) == e_failure)
    {
        red();
        printf("INFO : secret file extension not decoded...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : Secret file extension decoded successfully...!\n");
        reset();
        printf("\n");
    }

    if(decode_secret_file_size(decInfo) == e_failure)
    {
        red();
        printf("INFO : Secret file size is not decoded...!\n");
        reset();
        printf("\n");

        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : file size decoded successfully...!\n");
        reset();
        printf("\n");
    }

    if(decode_secret_file_data(decInfo) == e_failure)
    {
        red();
        printf("INFO : Secret file data not decoded...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : file data decoded successfully...!\n");
        reset();
        printf("\n");
    }
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_string[2];
    rewind(decInfo->fptr_stego_image);
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET); // Move to the pixel data

    if (decode_data_to_image(decInfo->fptr_stego_image, magic_string, 2) != e_success) 
    {
        return e_failure; // Handle decode error
    }
    magic_string[2] = '\0'; // Null terminate the string
    
    if(strcmp(magic_string, MAGIC_STRING))		//comparing magicstring not then failure
	{
        red();
	    printf("INFO: Magic String Not Matched\n");
        reset();
        printf("\n");
	    return e_failure;
	}

    return e_success;
}

Status decode_byte_to_lsb(char *image_buffer)
{
    char character = {0};
    for(int i=0; i<8; i++)
    {
        image_buffer[i]= image_buffer[i] & 1;					
        character = character << 1;											
        character = character | image_buffer[i];								
    }
    return character;
}

Status decode_data_to_image(FILE *fptr_image, char *data, int size)
{
    char image_buffer[8];

    for(int i = 0; i < size; i++)
    {
        fread(&image_buffer, sizeof(char), 8, fptr_image);

        data[i] = decode_byte_to_lsb(image_buffer);
    }

    return e_success;
}


Status decode_size_to_lsb(char *image_buffer)
{
    int size = 0;
    for(int i = 0; i < 32; i++)
    {
        image_buffer[i] = image_buffer[i] & 1;
        size = size << 1;
        size = size | image_buffer[i];
    }

    return size;
}


Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char extn_size_buffer[32];

    fread(extn_size_buffer, 32, 1, decInfo->fptr_stego_image);

    decInfo->extn_size = decode_size_to_lsb(extn_size_buffer);    

    return e_success;
}


Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    if (decode_data_to_image(decInfo->fptr_stego_image, decInfo->extn_output_secret_file, decInfo->extn_size) == e_failure) 
    {
        return e_failure; // Handle decode error
    }

    decInfo->extn_output_secret_file[decInfo->extn_size] = '\0';

    strcat(decInfo->output_secret_fname,decInfo->extn_output_secret_file); 

    if(strstr(decInfo -> output_secret_fname,".txt") != NULL)
    // Secret file (secret.txt)
    decInfo->fptr_output_secret = fopen(decInfo->output_secret_fname, "w");
    // Do Error handling
    if (decInfo->fptr_output_secret == NULL)
    {
        red();
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->output_secret_fname);
        reset();
        printf("\n");

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char file_size_buffer[32];

    fread(file_size_buffer, 32, 1, decInfo->fptr_stego_image);

    decInfo->size_output_secret_file = decode_size_to_lsb(file_size_buffer);    

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char file_data[decInfo->size_output_secret_file];

    decode_data_to_image(decInfo->fptr_stego_image, file_data, decInfo->size_output_secret_file);

    fwrite(file_data, decInfo->size_output_secret_file, 1, decInfo->fptr_output_secret);

    return e_success;
}

void print() 
{
    printf("Loading [");
    yellow();
    for (int i = 0; i < 40; i++)
    {
        printf(".");
        fflush(stdout);  // Force the output to appear immediately
        usleep(40000); // 60 milliseconds
    }
    reset();
    printf("] 100%%");
    printf("\n");
}