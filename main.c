#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

void reset();
void blue();
void green();
void red();
void cyan();

int main(int argc, char *argv[])
{
    // Check the argv[1] is -e or -d
    int op_type = check_operation_type(argv[1]);
    //Step 1: check the op_type is equal to e_encode
    //Step 2: If yes, then print encode started and start encoding
    //Step 3: If no, then check the op_type is equal to e_decode
    //Step 4: If yes, then print decode started and start decoding
    //Step 5: If no, return e_failure

    if(op_type == e_encode)
    {
        if(argc <4 )
        {
            //red();
            printf("INFO : Passed only %d arguments \n",argc);
            printf("Please pass atleast four arguments\n");
            //reset();
            //exit(0);
        }
        else if(argc > 5)
        {
            //red();
            printf("INFO : Passed  %d arguments \n",argc);
            printf("Please pass 4 or 5 arguments \n");
            //reset();
            //exit(0);
        }
        if(argc == 4 || argc == 5)
        {
            if(op_type == e_encode)
            {
                EncodeInfo encInfo;

                printf("\n");
                cyan();
                printf("        ####### Encoding Started  #######\n");
                reset();
                printf("\n");

                if(read_and_validate_encode_args(argv, &encInfo) == e_success)
                {
                    green();
                    printf("INFO : Read and Validation is Successfully completed\n");
                    reset();
                    printf("\n");
                    do_encoding( &encInfo );
                }
                else
                {
                    red();
                    printf("INFO : Read and validations are Failed \n");
                    reset();
                    printf("\n");
                }
            }
        }
    }

    if(op_type == e_decode)
    {
        if(argc < 4 || argc > 4)
        {
            red();
            printf("ERROR :: Passed only %d arguments\n",argc);
            printf("Please enter atleast 2 arguments like ./a.out -d stego_image.bmp outputfilename\n");
            return 0;
            reset();
        }/*
        if(argc > 4)
        {
            red();
            printf("ERROR :: Passed %d arguments\n ",argc);
            printf("Please pass 2 or 3 arguments only like ./a.out -d stego_image.bmp outputfilename\n");
            return 0;
            reset();
        }*/
        if(argc == 4)
        {
            if(op_type == e_decode)
            {
                DecodeInfo decInfo;

                printf("\n");
                cyan();
                printf("        ####### Decoding Started  #######\n");
                reset();
                printf("\n");

                if(read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
                {
                    green();
                    printf("INFO : Read and Validation is Successfully completed\n");
                    reset();
                    printf("\n");
                    do_decoding( &decInfo );
                }
                else
                {
                    red();
                    printf("INFO : Read and validations are Failed \n");
                    reset();
                    printf("\n");
                }
            }
        }
    }

    return 0;
}


OperationType check_operation_type(char *argv)    //We are only passing one argument so no need to collect it in array
{
    // Step 1: Check the argv is -e or not 
    // Step 2: If yes, then return e_encode 
    // Step 3: If no, go to step 4
    // Step 4: Check the argv is -d or not
    // Step 5: If yes, then return e_decode
    // Step 6: If no, return e_unsupported

    if( strstr(argv, "-e") != NULL)
    {
        return e_encode;
    }
    else if( strstr(argv, "-d") != NULL)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

