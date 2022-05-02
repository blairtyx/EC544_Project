#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rscombo.h"

void find_erasure(unsigned char codeword[], int csize, unsigned char eraselist[], int *numerase)
{
	for (int k = 0; k<csize; k++){
		if(codeword[k] == '~'){
			eraselist[*numerase] = csize - k - 1;
			(*numerase)++;
		}
	}
}


int main(int argc, char **argv)
{

	char msg[225] = "";
	char parity_hex[32] = "";
	if(argc != 3){
        return -1;
    }
    if(argc == 3){
		memcpy(msg, argv[1], 255);
		memcpy(parity_hex, argv[2], 32);
    }

	int ML = (sizeof (msg) + NPAR);
    unsigned char erasures[NPAR];
	int nerasures = 0,i;

	/********************* Hex string to unsigned char *********************/

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
	
	unsigned char fake_codeword[ML];
	memcpy(fake_codeword, msg, sizeof(msg));
	memcpy(fake_codeword+225, result, 16);

	find_erasure(fake_codeword, ML, erasures, &nerasures);
	// decode
	if ( rs_decode_data(fake_codeword, ML) ) {
		rs_correct_errors_erasures (fake_codeword,
				  ML,
				  nerasures,
				  erasures);

		printf("%s\n", fake_codeword);
	}

	return 0;
}
