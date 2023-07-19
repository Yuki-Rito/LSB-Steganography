#include <stdio.h>
#include <stdlib.h>
#include "decode.h"
#include "common.h"
#include <string.h>
#include "types.h"

// Function Definition - To read and validate arguments
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // Checking for default file is having file extention is .bmp
    if((argv[2] != NULL ) && strcmp(argv[2],".bmp"))
    {
	decInfo->src_image_fname = argv[2];
    }
    else
    {
	return e_failure;
    }

    // Checking for the secret file is having file extention .txt
    if((argv[3] != NULL) && strcmp(argv[3],".txt"))
    {
	decInfo->secret_fname = argv[3];
    }
    else
    {
	decInfo->secret_fname = "decode.txt";
    }
    return e_success;
}

// Function Definition - do decoding
Status do_decoding(DecodeInfo *decInfo)
{
    /* function call for open files */
    if( open_file(decInfo) == e_success)
    {
	printf("INFO: ## Decoding Procedure Started ##\n");
	printf("INFO: Opening required files\n");
	printf("INFO: Opened steged_img.bmp\n");
	printf("INFO: Decoding Magic String Sign\n");


	/* function call for open files */
	if(decode_magic_string(MAGIC_STRING, decInfo) == e_success)
	{
	    printf("INFO: Decoding Magic String Signature\n");
	    printf("INFO: Done\n");
	    /* function call for docode extn size */
	    if(decode_extn_size(decInfo)  == e_success)
	    {
		printf("INFO: Decoding Output File Extenstion\n");
		printf("INFO: Done\n");
		printf("INFO: Output File not mentioned. Creating decoded.txt as default\n");
		printf("INFO: Opened decoded.txt\n");
		printf("INFO: Done. Opened all required file\n");
		/* function call for decode secret file extn*/
		if(decode_secret_file_extn(decInfo) == e_success)
		{
		    printf("INFO: Decoded secret file extn successfully\n");
		    /* function call for decode secret file size*/
		    if(decode_secret_file_size(decInfo) == e_success)
		    {
			printf("INFO: Decoding File Size\n");
				printf("INFO: Done\n");
				printf("INFO: Decoding File Data\n");
				printf("INFO: Done\n");
			/* function call for decode secret file data*/
			if(decode_secret_file_data(decInfo) == e_success )
			{
			    printf("INFO: Decoding Done \n");
			}
			else
			{
			    printf("INFO: Failed to decode secret data\n");
			}
		    }
		    else
		    {
			printf("INFO: Failed to decode secret file size\n");
		    }
		}
		else
		{
		    printf("INFO: Failed to decode secret file extn\n");
		}
	    }
	    else
	    {
		printf("INFO: Failed to decode secret file extn size\n");

	    }
	}
	else
	{
	    printf("INFO: Failed to decode magic string\n");
	}
    }
    else
    {
	printf("INFO: Failed to open files\n");
    }
    return e_success;
}

// Function Definition - open files
Status open_file(DecodeInfo *decInfo)
{
    decInfo->fptr_src_image  = fopen(decInfo->src_image_fname,"r");

    if( decInfo->fptr_src_image == NULL )
    {
	return e_failure;
    }

    decInfo->fptr_secret = fopen(decInfo->secret_fname,"w");
    if(decInfo->fptr_secret == NULL)
    {
	return e_failure;
    }
    return e_success;
}

// Function Definition - magic string
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    fseek(decInfo->fptr_src_image,54,SEEK_SET);

    char magic_buffer[3];
    magic_buffer[2] = '\0';

    for(int i  = 0; i < 2; i++)
    {
	decode_data_from_image(&magic_buffer[i],8,decInfo->fptr_src_image);
    }

    if(strcmp(magic_buffer,magic_string) == 0)
    {
	printf("INFO: Magic string = %s\n",magic_buffer);
	return e_success;
    }
    else
	return e_failure;
}

// Function Definition - decoding data from image
Status decode_data_from_image(char *data, int size, FILE *fptr_src_image)
{
    char temp;
    *data = 0;

    for(int i = 0 ; i < size ; i++)
    {
	fread(&temp , 1 , 1 , fptr_src_image);
	temp = temp & 0x01;
	*data |= (temp << (7 - i ));
    }
}

// Function Definition - decode extn size
Status decode_extn_size(DecodeInfo *decInfo)
{
    char buffer[32], ch;
    int size  = 0;
    fread(buffer, 1, 32, decInfo->fptr_src_image);

    for(int i = 0; i < 32; i++)
    {
	ch  = buffer[i]&0x01;
	size = (size << 1) | ch;    
    }
    decInfo->extn_size = size;
    printf("INFO: Secret file extension size = %d\n",size);
    return e_success;
}

// Function Definition - decode secret file extn
Status decode_secret_file_extn(DecodeInfo *decInfo)
{

    int i;
    for(i = 0; i < decInfo->extn_size; i++)
    {
	decode_data_from_image(&decInfo->extn_secret_file[i],8,decInfo->fptr_src_image);
    }
    decInfo->extn_secret_file[i] = '\0';
    printf("INFO: Secret file extension = %s\n", decInfo->extn_secret_file);
    return e_success;
}

// Function Definition - decode secret file size
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32], ch;
    int size  = 0;
    fread(buffer,1,32,decInfo->fptr_src_image);

    for(int i = 0; i < 32; i++)
    {
	ch  = buffer[i]&0x01;
	size = (size << 1) | ch;    
    }

    decInfo->size_secret_file = size;
    printf("Secret File size = %d\n",size);
    return e_success;
}

// Function Definition - decode secret file data
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    for( int i = 0; i < decInfo->size_secret_file; i++)
    {
	decode_data_from_image(&ch,8,decInfo->fptr_src_image);
	//printf("Secret file data = %c\n", ch);
    }
    return e_success;
}
