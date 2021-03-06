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
// #define DEBUG


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
    while (1) {
        message_t *t_message = mpool.try_alloc();
        while (i < array_size){
            xx = acc.getX(x) * 90.0;
            yy = acc.getY(y) * 90.0;
            t_message->x[i] = xx;
            t_message->y[i] = yy;
            i++;   
            ThisThread::sleep_for(10ms);
        }
        queue.try_put(t_message);
        i = 0;
        ThisThread::sleep_for(860ms);
    }
}

/************************* Reed-Soloman Codec *************************/
char msg[array_size * 16+1];
#define ML (sizeof (msg) + NPAR)
unsigned char codeword[ML];



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



    /****************** Init Ethernet and TCP socket ******************/
    tr_info("Connecting to the network...");
    EthernetInterface       eth;
    TCPSocket               socket_0; // socket 0
    TCPSocket               socket_1; // socket 1 
    char                    sbuffer_0[256]; // send buffer 0 
    char                    sbuffer_1[256]; // send buffer 1
#ifdef TRANSMITION
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
            tr_info("[%d] Get From message queue, Queue Count: %d", run, queue_ct);
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
#ifdef DEBUG
            tr_info("[%d] size: %d, msg is : %s", run, char_ct, msg);
#endif
            rs_encode_data((unsigned char*)msg, sizeof(msg), codeword);
            tr_info("[%d] origin msg size: %d, codeword size: %d", run, sizeof(msg), sizeof(codeword));
            sprintf(sbuffer_0, "%04d", run);
            sprintf(sbuffer_1, "%04d", run);
            memcpy(sbuffer_0+4, msg, sizeof(msg));
            memcpy(sbuffer_1+4, codeword, sizeof(codeword));
            tr_info("[%d] Finish memory copy", run);
#ifdef DEBUG
            tr_info("[%d] size of sbuffer_1: %d", run, sizeof(sbuffer_1));
            printf("Try hex representation : \n");
            for (int i = 0; i<sizeof(msg); i++){
                printf("%02X", msg[i]);
            }
            printf("\n\n");
            for (int i = 0; i<sizeof(codeword); i++){
                printf("%02X", codeword[i]);
            }
            printf("\n\n");
            for (int i = 229; i<245; i++){
                printf("%02X", sbuffer_1[i]);
            }
            printf("\n\n");
            for (int i = 0; i<sizeof(sbuffer_1); i++){
                printf("%02X", sbuffer_1[i]);
            }
            printf("\n");
#endif

#ifdef TRANSMITION
            scount = socket_0.send(sbuffer_0, sizeof sbuffer_0);
            tr_info("[T] sent %d to socket_0", scount);
            scount = socket_1.send(sbuffer_1, sizeof sbuffer_1);
            tr_info("[T] sent %d to socket_1", scount);
            tr_info("[%d] End of one message passing", run);
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

