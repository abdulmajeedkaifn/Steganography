#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/*
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;  //To store the source file 'name' beautiful.bmp
    FILE *fptr_src_image;   //To store the address of source file beautiful.bmp
    uint image_capacity;    //To store the size of .bmp file
    //uint bits_per_pixel;
    //char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char *secret_fname;  //To store name of secret.txt
    FILE *fptr_secret;   //To store address of secret.txt
    char extn_secret_file[MAX_FILE_SUFFIX]; //Used to store the extension of our secret file
    //char secret_data[MAX_SECRET_BUF_SIZE];
    long size_secret_file;  //To store the size of our secret file

    /* Stego Image Info */
    char *stego_image_fname;    //To store name of output.bmp(name of image generated after encoding)
    FILE *fptr_stego_image;     //To store address of output.bmp(name of image generated after encoding)

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
//We are only passing one argument so no need to collect it in array
//OperationType check_operation_type(char *argv[]);
OperationType check_operation_type(char *argv);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size (secret file size) */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo);

/* To encode secret file extension size */
Status encode_secret_file_extn_size(long extn_size, EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
// This is a generic function. we use it in magic string, extension and for file data
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* To Encode the size of extension and size of file into LSB of image data array */
Status encode_size_to_lsb(int data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);

#endif
