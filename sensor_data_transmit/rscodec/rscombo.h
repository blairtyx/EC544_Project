#include "mbed-os/mbed.h"

#define NPAR 16

/* Reed Solomon encode/decode routines */
void  rs_init(void);

void rs_encode_data (unsigned char msg[], int nbytes, unsigned char dst[]);
unsigned char gmult(unsigned char a, unsigned char b);


//#define DUMP_ARRAYS		//generate data file, must do if NPAR changed
#define USE_HARDCODED		//use data file
#define USE_ENCODE  			//compile encoder routines

#define RS_DATA_FILE "rs_data.c"     //polynomial data file

/* Maximum degree of various polynomials. */
#define MAXDEG (NPAR*2)

#include RS_DATA_FILE


/********** polynomial arithmetic *******************/

/* multiplication using logarithms */
unsigned char gmult(unsigned char a, unsigned char b)
{
  unsigned char i,j;
  if (a==0 || b == 0) return (0);
  i = glog[a];
  j = glog[b];
  return (gexp[i+j]);
}


/* Simulate a LFSR with generator polynomial for n byte RS code.
 * Pass in a pointer to the data array, and amount of data.
 *
 * The parity bytes are deposited into pBytes[], and the whole message
 * and parity are copied to dest to make a codeword.
 *
 */
void rs_encode_data (unsigned char msg[], int nbytes, unsigned char dst[])
{
    int i,  j;
    unsigned char LFSR[NPAR+1], dbyte;

    // init LFSR to all zero
    memset(LFSR, 0, sizeof(LFSR));

    for (i = 0; i < nbytes; i++) {
	    dbyte = msg[i] ^ LFSR[NPAR-1];
	    for (j = NPAR-1; j > 0; j--) {
		    LFSR[j] = LFSR[j-1] ^ gmult(genPoly[j], dbyte);
	    }
	    LFSR[0] = gmult(genPoly[0], dbyte);
    }

	memcpy(dst, msg, nbytes);	//always in bytes
	for (i = 0; i < NPAR; i++)
	{
		dst[i+nbytes] = LFSR[NPAR-1-i];
	}
}

/* Initialize lookup tables, polynomials, etc.
	Optional if USE_HARDCODED #defined.
*/
void  rs_init(void)
{

}
