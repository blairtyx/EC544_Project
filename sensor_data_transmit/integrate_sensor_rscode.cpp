#include "mbed.h"
#include "rscombo.h"
#include "mbed-trace/mbed_trace.h"
#include "FXOS8700Q.h"
#include <cstring>


#define TRACE_GROUP "Main"

// sensor related
I2C                         i2c( PTE25, PTE24 );
FXOS8700QAccelerometer      acc( i2c, FXOS8700CQ_SLAVE_ADDR1 );
#define threshold 45.0

/*************** 
* Pass sensor data to main process through message queue 
***************/
#define array_size 14 // record pairs in one batch

typedef struct {
    float x[array_size];
    float y[array_size];
    float count;
} message_t;// one batch of message

MemoryPool<message_t, 16>   mpool;
Queue<message_t, 16>        queue;
Thread                      thread;

// reed-soloman codec reslated
char msg[array_size * 16];
#define ML (sizeof (msg) + NPAR)
unsigned char codeword[ML];


// flow control related
DigitalOut                  led_red(LED1);
DigitalOut                  led_green(LED2);
InterruptIn                 btn(BUTTON1);
bool TRANS_FLAG=0;
void button_pressed()
{
    led_red = !led_red;
    led_green = !led_green;
    TRANS_FLAG = !TRANS_FLAG;
}


// reed-soloman test code
void insert_erasure(unsigned char codeword[], 
                    int csize, 
                    int eloc,  
                    unsigned char eraselist[], 
                    int *numerase)
{
	codeword[ eloc - 1 ] = '~';
	eraselist[*numerase] =  csize - eloc;
	(*numerase)++;
}

/* Send Thread */
void send_thread(void)
{
    uint32_t i = 0;
    float x, y;
    float th, xx, yy;
    th = threshold*(-1);
    xx = x*90.0;
    yy = y*90.0;
    while (1) {
        if(TRANS_FLAG){
            message_t *t_message = mpool.try_alloc();
            while (i < array_size){
                xx = acc.getX(x) * 90.0;
                yy = acc.getY(y) * 90.0;
                if (yy > th || xx > threshold || xx < th)
                {
                    t_message->x[i] = xx;
                    t_message->y[i] = yy;
                    i++;
                }     
            }
            queue.try_put(t_message);
            i = 0;
            ThisThread::sleep_for(1000ms);
        }
    }
}

// main() runs in its own thread in the OS
int main()
{
    
    mbed_trace_init();
    tr_info("Start Reed-Soloman Test");
    led_green = 1; // turn off green led 
    
    acc.enable();// Enable the accelerometer

    tr_info("NPAR: %d, ML: %d", NPAR, ML);
    unsigned char erasures[NPAR];
    int nerasures, i ;

    // Initialize the RS library
    rs_init();

    /* Encode data into codeword, adding NPAR parity bytes */
	// rs_encode_data(msg, sizeof(msg), codeword);
    // tr_info("Original data is:\"%u\"\n", msg);
    // tr_info("Encoded data is: \"%u\"\n", codeword); // note that the tailing part is not shown

    btn.fall(&button_pressed);
    thread.start(callback(send_thread));
    int run = 0;
    char tmp[32];
    while (true) {
        message_t *message;
        if (queue.try_get(&message)) {
            int char_ct = 0;
            int queue_ct = queue.count();
            tr_info("Queue Count: %d", queue_ct);
            // message_t *message = (message_t *)evt.value.p; // pass the return pointer 
            for (int i = 0; i < array_size; i++){
                // tr_info("x[%d]: %f, y[%d]: %f", i, message->x[i], i, message->y[i]);
                sprintf(tmp, "(%6.3f,%6.3f),", message->x[i], message->y[i]);
                // tr_info("[%d] tmp is : %s", i, tmp);
                strcat(msg, tmp);
                char_ct += strlen(tmp);
                // memcpy(&msg[i*32], tmp, sizeof tmp);
            }
            if (char_ct > 256){
                tr_error("TOO LONG FOR RSCODE: %d", char_ct);
            }
            tr_info("[%d] size: %d, msg is : %s", run, char_ct, msg);
            rs_encode_data((unsigned char*)msg, sizeof(msg), codeword);
            tr_info("[%d] Encoded data is: %u", run, codeword); 
            tr_info("[%d] origin msg size: %d, codeword size: %d", run, sizeof(msg), sizeof(codeword));
            tr_info("[%d] End of one message passing", run);
            mpool.free(message);
            run++;
            memset(msg, 0, sizeof msg);
        }
    }

end:
    tr_info("End of demo");
}

