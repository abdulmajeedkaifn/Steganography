#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * decoding secret file from source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    char *stego_image_fname;    //To store name of output.bmp(name of image generated after encoding)
    FILE *fptr_stego_image; 

    /* Secret File Info */
    char *output_secret_fname; //To store name of secret.txt
    FILE *fptr_output_secret;   //To store address of secret.txt
    char extn_output_secret_file[MAX_FILE_SUFFIX]; //Used to store the extension of our secret file
    //char secret_data[MAX_SECRET_BUF_SIZE];
    long extn_size;
    long size_output_secret_file;  //To store the size of our secret file

} DecodeInfo;


/* Decoding function prototype */

/* Check operation type */
//We are only passing one argument so no need to collect it in array
//OperationType check_operation_type(char *argv[]);
OperationType check_operation_type(char *argv);

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(int argc, char* argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Open files in r and w mode as per requirements*/
Status open_files_for_decode(DecodeInfo *decInfo);

/* To decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
// This is a generic function. we use it in magic string, extension and for file data
Status decode_data_to_image(FILE *fptr_stego_image, char *data, int size);

/* decode a byte from LSB of image data array */
Status decode_byte_to_lsb(char *magic_buffer);

/* To decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *encInfo);

/* To decode the size of extension and size of file from LSB of image data array */
Status decode_size_to_lsb(char *extn_size_buffer);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *encInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *encInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif
