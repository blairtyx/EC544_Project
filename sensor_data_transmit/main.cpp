#include "mbed.h"
#include "SocketAddress.h"
#include "EthernetInterface.h"
#include "mbed-trace/mbed_trace.h"

DigitalOut led(LED1);

#define TRACE_GROUP "Main"

// main() runs in its own thread in the OS
int main()
{
    int ret;

    mbed_trace_init();
    tr_info("Connecting to the network...");

    EthernetInterface eth;
    SocketAddress a;
    TCPSocket socket;
    char sbuffer[64];

    // set the Ethernet Interface
    ret = eth.set_network(MBED_CONF_APP_LOCAL_HOST,
                        MBED_CONF_APP_LOCAL_MASK,
                        MBED_CONF_APP_LOCAL_GATEWAY); // static interface
    if (ret != 0) {
        tr_error("Set Network error: %x", ret);
        goto end;
    }
    ret = eth.connect();
    if (ret != 0) {
        tr_error("Connection error: %x", ret);
        goto end;
    }

    ret = eth.get_ip_address(&a);
    if (ret != 0) {
        tr_error("No IP address");
        goto end;
    }
    tr_info("The client IP address is '%s'\n", a.get_ip_address() ? a.get_ip_address() : "None");

    // Open a socket on the network interface, and create a TCP connection to MBED_CONF_APP_SERVER_HOST
    socket.open(&eth);

    // eth.gethostbyname("raspberrypi.local", &a);
    a.set_ip_address(MBED_CONF_APP_SERVER_HOST);
    a.set_port(MBED_CONF_APP_SERVER_PORT);
    socket.connect(a);
    if (ret != 0){
        tr_error("No IP address");
        goto end;
    }

    // Send a simple request
    for (int i=0; i<10; i++){
        sprintf(sbuffer, "Hi, im client No.%d\r\n", i);
        int scount = socket.send(sbuffer, sizeof sbuffer);
        printf("sent %d [%.*s]\n", scount, strstr(sbuffer, "\r\n") - sbuffer, sbuffer);
    }

    // Close the socket to return its memory and bring down the network interface
    socket.close();
    tr_info("Socket Closed");

    // Bring down the ethernet interface
    eth.disconnect();
    tr_info("Ethernet interface disconnected");

end:
    tr_info("End of the demo");
}

