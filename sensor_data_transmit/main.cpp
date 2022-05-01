#include "DigitalOut.h"
#include "InterruptIn.h"
#include "PinNames.h"
#include "TCPSocket.h"
#include "ThisThread.h"
#include "mbed.h"
#include "SocketAddress.h"
#include "EthernetInterface.h"
#include "mbed-trace/mbed_trace.h"
#include "rscombo.h"
#include "nsapi_types.h"
#include <cstdio>
#include "FXOS8700Q.h"
#include <ctype.h>


#define TRACE_GROUP "Main"

/************************* Code Flow Control *************************/
#define TRANSMITION


/************************* LED & Button *************************/
DigitalOut                  led_red(LED1); // Red LED 
DigitalOut                  led_green(LED2); // Green LED


/************************* Sensor *************************/
#define threshold 45.0
I2C                         i2c(PTE25, PTE24);
FXOS8700QAccelerometer      acc(i2c, FXOS8700CQ_SLAVE_ADDR1);


/************************* Message Queue *************************/
#define array_size 14 // record pairs in one batch
typedef struct {
    float x[array_size];
    float y[array_size];
    float count;
} message_t;// one batch of message

MemoryPool<message_t, 16>   mpool;
Queue<message_t, 16>        queue;
Thread                      thread;

void send_thread(void)
{
    uint32_t i = 0;
    float x, y;
    float th, xx, yy;
    led_red = !led_red;
    led_green = !led_green;
    th = threshold*(-1);
    xx = x*90.0;
    yy = y*90.0;
    while (1) {
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

/************************* Reed-Soloman Codec *************************/
char msg[array_size * 16];
#define ML (sizeof (msg) + NPAR)
unsigned char codeword[ML];

int printf_ByteArray(const unsigned char *data, size_t len) 
{
	size_t i;
	int result = 0;
	for (i = 0; i < len; i++) 
	{
		int y;
		int ch = data[i];
		static const char escapec[] = "\a\b\t\n\v\f\n\'\"\?\\";
		const char *p = strchr(escapec, ch);
		if (p && ch) 
		{
			static const char escapev[] = "abtnvfn\'\"\?\\";
			y = printf("\\%c", escapev[p - escapec]);
		} 
		else if (isprint(ch)) {
			y = printf("%c", ch);
		} else {
			// If at end of array, assume _next_ potential character is a '0'.
			int nch = i >= (len - 1) ? '0' : data[i + 1];
			if (ch < 8 && (nch < '0' || nch > '7')) 
			{
				y = printf("\\%o", ch);
			} else if (!isxdigit(nch)) {
				y = printf("\\x%X", ch);
			} else {
				y = printf("\\o%03o", ch);
			}
		}
		if (y == EOF)
		    return EOF;
		result += y;
	}
	printf("\n");
	return result;
}


/************************** Transmission **************************/
nsapi_error_t establish_eth(EthernetInterface &eth, const SocketAddress &ip_address, 
                        const SocketAddress &netmask, const SocketAddress &gateway)
{
    nsapi_error_t ret;
    SocketAddress localAddress;

    ret = eth.set_network(ip_address, netmask, gateway);
    if (ret != 0) 
    {
        tr_error("Set Network error: %x", ret);
        goto end;
    }
    ret = eth.connect();
    if (ret != 0) 
    {
        tr_error("Connection error: %x", ret);
        goto end;
    }
    tr_info("The client IP address is '%s'\n", localAddress.get_ip_address() ? localAddress.get_ip_address() : "None");
end: 
    return ret;
}

nsapi_error_t establish_TCP_socket(TCPSocket &socket, EthernetInterface &eth, const char *addr, 
                                uint16_t port)
{
    nsapi_error_t ret;
    SocketAddress a;
    socket.open(&eth);

    a.set_ip_address(addr);
    a.set_port(port);

    ret = socket.connect(a);
    if (ret != 0)
    {
        tr_error("TCP Socket Failed: %x", ret);
        goto end;
    }
    tr_info("TCP Socket Established at %s:%d", addr, port);
end:
    return ret;
}



/************************** Main **************************/
int main()
{

    /****************** Init ******************/
    int ret;
    led_green = 1; // turn off green led 
    mbed_trace_init();
    int run = 0;
    char tmp[32];
    /****************** End ******************/


    /****************** Digital Accelerometer ******************/
    tr_info("Config digital accelerometer");
    acc.enable();// Enable the accelerometer
    /****************** End ******************/



#ifdef TRANSMITION
    /****************** Init Ethernet and TCP socket ******************/
    tr_info("Connecting to the network...");
    EthernetInterface       eth;
    TCPSocket               socket_0; // socket 0
    TCPSocket               socket_1; // socket 1 
    char                    sbuffer_0[256]; // send buffer 0 
    char                    sbuffer_1[256]; // send buffer 1
    ret = establish_eth(eth, MBED_CONF_APP_LOCAL_HOST, MBED_CONF_APP_LOCAL_MASK, MBED_CONF_APP_LOCAL_GATEWAY);
    if (ret != 0) {
        tr_info("Failed in establishing Ethernet Interface");
        goto end;
    }
    ret = establish_TCP_socket(socket_0, eth, MBED_CONF_APP_SERVER_HOST_0, MBED_CONF_APP_SERVER_PORT_0);
    if (ret != 0) {
        tr_info("Failed in establishing TCP Socket 0");
        goto end;
    }
    ret = establish_TCP_socket(socket_1, eth, MBED_CONF_APP_SERVER_HOST_1, MBED_CONF_APP_SERVER_PORT_1);
    if (ret != 0) {
        tr_info("Failed in establishing TCP Socket 1");
        goto end;
    }
    /****************** End ******************/
#endif



    /****************** Control Logic and temp variables ******************/
    thread.start(callback(send_thread));
    tr_info("Config callback function");
    /****************** End ******************/


    /****************** Read Sensor, Encode, Transmit ******************/
    while (true) {
        message_t *message;
        if (queue.try_get(&message)) {
            int char_ct = 0;
            int queue_ct = queue.count();
            int scount;
            tr_info("Queue Count: %d", queue_ct);
            // message_t *message = (message_t *)evt.value.p; // pass the return pointer 
            for (int i = 0; i < array_size; i++){
                // tr_info("x[%d]: %f, y[%d]: %f", i, message->x[i], i, message->y[i]);
                sprintf(tmp, "(%6.2f,%6.2f),", message->x[i], message->y[i]);
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
            // printf_ByteArray(codeword, sizeof(codeword));
            tr_info("[%d] origin msg size: %d, codeword size: %d", run, sizeof(msg), sizeof(codeword));
            tr_info("[%d] End of one message passing", run);
#ifdef TRANSMITION
            sprintf(sbuffer_0, "%04d", run);
            sprintf(sbuffer_1, "%04d", run);
            memcpy(sbuffer_0+4, msg, sizeof(msg));
            memcpy(sbuffer_1+4, msg, sizeof(codeword));
            tr_info("[%d] Finish memory copy", run);
            printf_ByteArray((unsigned char*)sbuffer_0, sizeof(sbuffer_0));
            printf_ByteArray((unsigned char*)sbuffer_1, sizeof(sbuffer_1));

            scount = socket_0.send(sbuffer_0, sizeof sbuffer_0);
            tr_info("[T] sent %d [%.*s]\n", scount, strstr(sbuffer_0, "\r\n") - sbuffer_0, sbuffer_0);
            scount = socket_1.send(sbuffer_1, sizeof sbuffer_1);
            tr_info("[T] sent %d [%.*s]\n", scount, strstr(sbuffer_1, "\r\n") - sbuffer_1, sbuffer_1);

#endif
            mpool.free(message);
            run++;
            memset(msg, 0, sizeof msg);
        }
    }
    /****************** End ******************/

#ifdef TRANSMITION
    /****************** Close Socket and Ethernet Interface ******************/
    socket_0.close();
    tr_info("Socket0 Closed");
    socket_1.close();
    tr_info("Socket1 Closed");
    eth.disconnect();
    tr_info("Ethernet interface disconnected");
    /****************** End ******************/
#endif


end:
    tr_info("End of the demo");
}

