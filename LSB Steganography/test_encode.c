#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"
/* 
Name	          : Enba Kumar R
Date	          : 09.07.2023
Description 	  : LSB Steganography (project)
Sample Exectution : ENCODING

./a.out -e beautiful.bmp secret.txt
INFO: Selected encoding
INFO: Read and validate encode arguments is success
INFO: Opening required filesnINFO: Opened SkeletonCode/beautiful.bmp
INFO: Opened secret.txt
INFO: Opened steged_img.bmp
INFO: Done
INFO: ## Encoding Procedure Started ##
INFO: Check capacity is success
INFO: Copied bmp header successful
INFO: Encoded magic string Signature is successful
INFO: Encoded secret file extension size is succesful
INFO: Encoding secret.txt File Extenstion
INFO: Done
INFO: Encoded secret file size is successful
INFO: Encoded secret file data is successful
INFO: Copied remaining image data successful
INFO: ## Encoding Done Successfully ##

DECODING
./a.out -d stego.bmp
INFO: Selected decoding
INFO: Read and validate decode is success
INFO: ## Decoding Procedure Started ##
INFO: Opening required files
INFO: Opened steged_img.bmp
INFO: Decoding Magic String Sign
INFO: Magic string = #*
INFO: Decoding Magic String Signature
INFO: Done
INFO: Secret file extension size = 4
INFO: Decoding Output File Extenstion
INFO: Done
INFO: Output File not mentioned. Creating decoded.txt as default
INFO: Opened decoded.txt
INFO: Done. Opened all required file
INFO: Secret file extension = .txt
INFO: Decoded secret file extn successfully
Secret File size = 0
INFO: Decoding File Size
INFO: Done
INFO: Decoding File Data
INFO: Done
INFO: Decoding Done 
INFO: ## Decoding is successfully completed ##

 */



//input argument validation
int main(int argc, char *argv[])
    //option validation for encoding
{
    if(check_operation_type(argv) == e_encode)
    {
	printf("INFO: Selected encoding\n");
	EncodeInfo encInfo;

	if(read_and_validate_encode_args(argv, &encInfo) == e_success)
	{    //read the file name
	    printf("INFO: Read and validate encode arguments is success\n");

	    // Encoding secret data
	    if(do_encoding(&encInfo) == e_success)
	    {
		printf("INFO: ## Encoding Done Successfully ##\n");
				    
	    }
	    else
	    {
		printf("Failed to encode\n");
		return -1;
	    }
	}
	else
	{
	    printf("INFO: Read and validate encode arguments is a failue\n");
	    return -1;
	}

    }
    //decoding
    else if(check_operation_type(argv) == e_decode)
    {
	printf("INFO: Selected decoding\n");
	DecodeInfo decInfo;

	//To read and store file names
	if(read_and_validate_decode_args(argv, &decInfo) == e_success)
	{
	    printf("INFO: Read and validate decode is success\n");

	    // Decode the stego image
	    if(do_decoding(&decInfo) == e_success)
	    {
		printf("INFO: ## Decoding is successfully completed ##\n");
	    }
	    else
	    {
		printf("Failed to decode\n");
		return -1;
	    }
	}
	else
	{
	    printf("Read and validate decode arguments is a failure\n");
	    return -1;
	}
    }
    else
    {
	printf("Invalid Option!!\nUsage for encoding:\n./a.out -e beautiful secret.txt [stego.bmp]\n");
	printf("Usage for decoding:\n./a.out -d stego.bmp [decode.txt]\n");
    }
    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") ==0)
	return e_encode;
    else if(strcmp(argv[1], "-d") == 0)
	return e_decode;
    else
	return e_unsupported;
}
