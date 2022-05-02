#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rscombo.h"
#include <sqlite3.h> 



void insert_erasure(unsigned char codeword[], int csize, int eloc,  unsigned char eraselist[], int *numerase)
{
	codeword[ eloc - 1 ] = '~';
	eraselist[*numerase] =  csize - eloc;
	(*numerase)++;
}


int main(int argc, char **argv)
{

	char msg[225] = "";

	char parity_hex[32] = "";
	if(argc != 3)
    {
        return -1;
    }
    if(argc == 3){
		memcpy(msg, argv[1], 255);
		memcpy(parity_hex, argv[2], 32);
    }

	int ML = (sizeof (msg) + NPAR);
	unsigned char codeword[ML];
	printf("ML: %d\n", ML);
    unsigned char erasures[NPAR];
	int nerasures,i;

	/********************* Encode and decode using real method *********************/
	rs_encode_data((unsigned char *)msg, sizeof(msg), codeword);

	// debug
	printf("Origin data is: %s\n", msg);
	printf("Encoded data is: %s\n", codeword);
	printf("Try hex representation : \n");
	printf("size of msg: %d size of codeword: %d\n", sizeof(msg), sizeof(codeword));
	for (int i = 0; i<sizeof(msg); i++){
		printf("%02X", msg[i]);
	}
	printf("\n\n");
	for (int i = 0; i<sizeof(codeword); i++){
		printf("%02X", codeword[i]);
	}
	printf("\n\n");
	for (int i = 225; i<sizeof(codeword); i++){
		printf("%02X", codeword[i]);
	}
	printf("\n");


	// insert erasure
	for(nerasures = 0, i=1; i <= NPAR; i++)
	{
		insert_erasure(codeword, ML, i, erasures, &nerasures);
	}
	printf("with erasures: \"%s\"\n", codeword);


	// decode
	if ( rs_decode_data(codeword, ML) ) {
		rs_correct_errors_erasures (codeword,
				  ML,
				  nerasures,
				  erasures);

		printf("Corrected codeword: \"%s\"\n", codeword);
	}
	else
	{
		printf("No Errors\n");
	}
	if( strcmp((char*)msg,(char*)codeword) )
	{
		printf("Error in corrected compare\n");
	}


	/********************* Hex string to unsigned char *********************/
	printf("\n********************* Test Fake String *********************\n");

	char *p;
	int cnt = (strlen(parity_hex) + 1) /2;
	unsigned char *result = (unsigned char *) malloc(cnt), *r;
	unsigned int c;
	for (p = parity_hex, r = result; *p; p += 2) {
		if (sscanf(p, "%02X", (unsigned int *)&c) != 1) {
			break; // Didn't parse as expected
		}
		*r++ = c;
	}
	for (i = 0; i < 16; i++){
    	printf("%02X",result[i]);
	}
	printf("\n");
	
	unsigned char fake_codeword[ML];
	memcpy(fake_codeword, msg, sizeof(msg));
	memcpy(fake_codeword+225, result, 16);

	for (int i = 0; i<sizeof(fake_codeword); i++){
		printf("%02X", fake_codeword[i]);
	}
	printf("\n\n");
	
	return 0;


}
