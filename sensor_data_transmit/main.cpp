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

// #define TEST_BUTTON
#define TEST_TRANSMIT
#define TEST_DUAL_TRANSMIT
// #define TEST_CODEC

bool TRANS_FLAG=0;

//set red LED on
DigitalOut led_red(LED1);
DigitalOut led_green(LED2);
// button 
InterruptIn btn(BUTTON1);

#define TRACE_GROUP "Main"




/* Test the Reed-Soloman Codec */
#if defined(TEST_CODEC)
unsigned char msg[] = "Nervously I loaded the twin ducks aboard the revolving platform. \
This is mainly the suppliment info to rep12123";

#define ML (sizeof (msg) + NPAR)

unsigned char codeword[ML];

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
#endif





/* Test Transmission over TCP socket */
#if defined(TEST_TRANSMIT)
nsapi_error_t establish_eth(EthernetInterface &eth, 
                            const SocketAddress &ip_address, 
                            const SocketAddress &netmask, 
                            const SocketAddress &gateway){
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

nsapi_error_t establish_TCP_socket(TCPSocket &socket, 
                                EthernetInterface &eth, 
                                const char *addr, 
                                uint16_t port){
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

#endif




/* Test the act of the button */
#if defined(TEST_BUTTON)
void button_pressed()
{
    led_red = !led_red;
    led_green = !led_green;
    TRANS_FLAG = !TRANS_FLAG;
}
#endif

// main() runs in its own thread in the OS
int main()
{
    int ret;
    led_green = 1; // turn off green led 
    mbed_trace_init();
    tr_info("Connecting to the network...");

#if defined(TEST_BUTTON)
    tr_info("Config digital accelerometer");
    btn.fall(&button_pressed);
    while(1)
    {
        if (TRANS_FLAG == 1){
            tr_info("button preessed");
        }
    }
#endif



#if defined(TEST_CODEC)
    tr_info("NPAR: %d, ML: %d", NPAR, ML);
    unsigned char erasures[NPAR];
    int nerasures, i ;

    /* Encode data into codeword, adding NPAR parity bytes */
	rs_encode_data(msg, sizeof(msg), codeword);
    tr_info("Original data is:\"%u\"\n", msg);
    tr_info("Encoded data is: \"%u\"\n", codeword); // note that the tailing part is not shown

    // simulate erasure 
	for(nerasures = 0, i=1; i <= NPAR; i++)
	{
		insert_erasure(codeword, ML, i*2, erasures, &nerasures);
	}
	tr_info("with erasures: \"%s\"\n", codeword);
#endif


#if defined(TEST_TRANSMIT)
    // init Ethernet and TCP socket
    EthernetInterface eth;
    TCPSocket socket_0;
    char sbuffer_0[64]; // send buffer

#if defined(TEST_DUAL_TRANSMIT)
    char sbuffer_1[64];
    TCPSocket socket_1;
#endif 
    // set the Ethernet Interface
    ret = establish_eth(eth, MBED_CONF_APP_LOCAL_HOST,
                        MBED_CONF_APP_LOCAL_MASK,
                        MBED_CONF_APP_LOCAL_GATEWAY);
    if (ret != 0) {
        tr_info("Failed in establishing Ethernet Interface");
        goto end;
    }

    // set the TCP Socket
    ret = establish_TCP_socket(socket_0, eth, MBED_CONF_APP_SERVER_HOST_0, MBED_CONF_APP_SERVER_PORT_0);
    if (ret != 0) {
        tr_info("Failed in establishing TCP Socket 0");
        goto end;
    }
#if defined(TEST_DUAL_TRANSMIT)
    ret = establish_TCP_socket(socket_1, eth, MBED_CONF_APP_SERVER_HOST_1, MBED_CONF_APP_SERVER_PORT_1);
    if (ret != 0) {
        tr_info("Failed in establishing TCP Socket 1");
        goto end;
    }
#endif

    // Send a simple request
    for (int i=0; i<10; i++){
        int scount;
        sprintf(sbuffer_0, "[Server0] Hi, im client No.%d", i);
        scount = socket_0.send(sbuffer_0, sizeof sbuffer_0);
        printf("sent %d [%.*s]\n", scount, strstr(sbuffer_0, "\r\n") - sbuffer_0, sbuffer_0);
        ThisThread::sleep_for(100ms);
#if defined(TEST_DUAL_TRANSMIT)
        sprintf(sbuffer_1, "[Server1] Hi, im client No.%d", i);
        scount = socket_1.send(sbuffer_1, sizeof sbuffer_1);
        printf("sent %d [%.*s]\n", scount, strstr(sbuffer_1, "\r\n") - sbuffer_1, sbuffer_1);
#endif
    }

    // Close the socket to return its memory and bring down the network interface
    socket_0.close();
    tr_info("Socket0 Closed");

#if defined(TEST_DUAL_TRANSMIT)
    socket_1.close();
    tr_info("Socket1 Closed");
#endif

    // Bring down the ethernet interface
    eth.disconnect();
    tr_info("Ethernet interface disconnected");
#endif
end:
    tr_info("End of the demo");
}

