#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include "decode.h"

void red();
void green();
void yellow();
void reset();
void print();

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
        red();
    	perror("fopen"); // perror will give the reason why file is not opening
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
        reset();
        printf("\n");

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
        red();
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
        reset();
        printf("\n");

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
        red();
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
        reset();
        printf("\n");

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    //Step 1: check the argv[2] is .bmp file or not
    //Step 2: If yes, Store the source filename into src_image_fname
    //Step 3: If no, then print error message and return e_failure
    //Step 4: Check argv[3] is .txt file
    //Step 5: If yes, Store the secret filename into secret_fname and also store the secret file extension
    //        into extn_secret_file (Note: use strcpy for char array)
    //Step 6: If no, then print error message and return e_failure
    //Step 7: Check argv[4] is passed or not, If no go to step 11
    //Step 8: If yes, then check that file is .bmp file or not, If no print error message and return failure
    //Step 9: If yes store the filename into stego_image_fname
    //Step 10: If no, Then print error message
    //Step 11: If argv[4] is not passed then create the default filename with .bmp then store the file
    //         name into stego_image_fname
    //Step 12: Return e_success after successfull validation

    if(strstr(argv[2], ".bmp"))
    {
        encInfo -> src_image_fname = argv[2];

        if( strstr(argv[3], ".txt"))
        {
            encInfo -> secret_fname = argv[3];
            //printf("Secretfname: %s\n", encInfo->secret_fname);
            strcpy(encInfo -> extn_secret_file, ".txt");

            if( argv[4] != NULL)
            {
                if(strstr(argv[4], ".bmp"))
                {
                    encInfo -> stego_image_fname = argv[4];
                }
                else
                {
                    red();
                    printf("INFO : Please pass valid arguments.\n");
                    reset();
                    printf("\n");
                    return e_failure;
                }
            }
            else
            {
                encInfo -> stego_image_fname = "stego_image.bmp";
                encInfo -> fptr_stego_image = fopen(encInfo -> stego_image_fname, "w");
                green();
                printf("INFO : Output file is not mentioned, default file name is added.\n");
                reset();
                printf("\n");
            }
        }
        else
        {
            red();
            printf("INFO : Please pass valid arguments.\n");
            reset();
            printf("\n");
            return e_failure;
        }
    }
    else
    {
        red();
        printf("INFO : Please pass valid arguments.\n");
        reset();
        printf("\n");
        return e_failure;
    }

    return e_success;
}

uint get_file_size(FILE *fptr)
{
    //Move the pos to end of the file
    //Then  return the size

    long int count = 0;

    for( ; fgetc(fptr) != EOF; )
        count++;

    return count;
}

Status check_capacity(EncodeInfo *encInfo)
{
    //Find the beautiful.bmp size
    encInfo -> image_capacity = get_image_size_for_bmp(encInfo -> fptr_src_image);

    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);

    //check the capacity of bmp file
    /*image capacity > (54 + 16 + 32 + extn + file_size)*/

    if(encInfo -> image_capacity > (54 + strlen(MAGIC_STRING)*8 + 32 + strlen(encInfo -> extn_secret_file)*8 + 32 + (encInfo -> size_secret_file)*8 ))
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    //Declare one array
    char buffer[54];

    /*Rewind the fptr_src_image because its offest is in 26th position after reading width and height -
        in get_image_size_for_bmp function*/
    rewind(fptr_src_image);

    //Read the first 54 bytes from src_image and store into buffer using fread
    fread(buffer, sizeof(char), 54, fptr_src_image);

    //write that 54 bytes into fptr_dest_image using fwrite
    fwrite(buffer, sizeof(char), 54, fptr_dest_image);

    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files( encInfo ) == e_failure) // No need to pass &encInfo again
    {
        print();
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

    if(check_capacity(encInfo) == e_failure)
    {
        print();
        red();
        printf("INFO : Image capaciy is small...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : Checking file capacity successfully completed...!\n");
        reset();
        printf("\n");
    }

    //Encoding Part

    if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_failure)
    {
        print();
        red();
        printf("INFO : bmp header not copied...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : copy bmp header is successfully completed...!\n");
        reset();
        printf("\n");
    }

    if(encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        print();
        red();
        printf("INFO : magic_string not encoded...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : magic_string encoded successfully...!\n");
        reset();
        printf("\n");
    }

    //call encode_secret_file_extn_size to encode secret file extension size
    if(encode_secret_file_extn_size(strlen(encInfo -> extn_secret_file), encInfo) == e_failure)
    {
        print();
        red();
        printf("INFO : Secret file extension size not copied...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : Secret file extension size encoded successfully...!\n");
        reset();
        printf("\n");
    }

    if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_failure)    //call encode_secret_file_extn(encInfo -> extn_secret_file, encInfo)
    {
        print();
        red();
        printf("INFO : secret file extension not copied...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : Secret file extension encoded successfully...!\n");
        reset();
        printf("\n");
    }

    if(encode_secret_file_size(encInfo -> size_secret_file, encInfo) == e_failure)    //call encode_secret_file_size( size of secret file, encinfo) == e_failure
    {
        print();
        red();
        printf("INFO : Secret file size is not encoded...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : file size encoded successfully...!\n");
        reset();
        printf("\n");
    }

    //Encoding secret file data into stego_image.bmp

    if(encode_secret_file_data(encInfo) == e_failure)
    {
        print();
        red();
        printf("INFO : Secret file data not encoded...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : file data encoded successfully...!\n");
        reset();
        printf("\n");
    }

    if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        print();
        red();
        printf("INFO : Remaining data is not encoded...!\n");
        reset();
        printf("\n");
        return e_failure;
    }
    else
    {
        print();
        green();
        printf("INFO : remaining data is encoded successfully...!\n");
        reset();
        printf("\n");
    }
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string, strlen(magic_string), encInfo -> fptr_src_image, encInfo -> fptr_stego_image);

    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    //Clear image buffer in the lsb bit
    //get the bit from msb in ch variable
    //set the bit in lsb side

    for(int i = 0; i < 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & ~1) | (unsigned) ((data & (1 << (7-i))) >> (7-i));
    }
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    //Declare one array 
    char image_buffer[8];
    //run a loop until size
    //read 8 bytes from src_image using fread
    //Call byte to lsb function (data[0],image_buffer)

    //fseek(fptr_src_image, 55, SEEK_SET);
    //fseek(fptr_stego_image, 55, SEEK_SET);

    for(int i = 0; i < size; i++)
    {
        fread(image_buffer, 8, sizeof(char), fptr_src_image);
        encode_byte_to_lsb(data[i], image_buffer);
        //write the 8 bytes to stego.bmp
        fwrite(image_buffer, 8, sizeof(char), fptr_stego_image);
    }
}

Status encode_size_to_lsb(int data, char *image_buffer)
{
    //Step 1: Clear the LSB bit in image buffer 
    //Step 2: Get MSB bit from extn_size
    //Step 3: Set the LSB bit in image buffer
    
    for(int i = 0; i < 32; i++)
    {
        image_buffer[i] = (image_buffer[i] & ~1) | (unsigned) ((data & (1 << (31-i))) >> (31-i));
    }
}

Status encode_secret_file_extn_size(long extn_size, EncodeInfo *encInfo)
{
    //Step 1: Declare a char array
    char image_buffer[32];

    //Step 2: Read 32 bytes from src_image
    fread(image_buffer, sizeof(char), 32, encInfo->fptr_src_image);

    //Step 3: call encode_size_to_lsb(extn_size, image_buffer)

    encode_size_to_lsb(extn_size, image_buffer);

    //Step 4: Write 32 bytes to stego_image
    fwrite(image_buffer, sizeof(char), 32, encInfo->fptr_stego_image);

    return e_success;
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    //call encode_data_to_image function

    encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image);

    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    //Step 1: Declare a char array
    char image_buffer[32];

    //Step 2: Read 32 bytes from src_image
    fread(image_buffer, sizeof(char), 32, encInfo->fptr_src_image);

    //Step 3: call encode_size_to_lsb(extn_size, image_buffer)

    encode_size_to_lsb(file_size, image_buffer);

    //Step 4: Write 32 bytes to stego_image
    fwrite(image_buffer, sizeof(char), 32, encInfo->fptr_stego_image);

    return e_success;
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char secret_buffer[encInfo->size_secret_file];

    rewind(encInfo->fptr_secret);

    fread(secret_buffer, encInfo -> size_secret_file, sizeof(char), encInfo->fptr_secret);

    encode_data_to_image(secret_buffer, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image);

    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[100];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, sizeof(char), sizeof(buffer), fptr_src)) > 0)
    {
        fwrite(buffer, sizeof(char), bytes_read, fptr_dest);
    }

    return e_success;
}