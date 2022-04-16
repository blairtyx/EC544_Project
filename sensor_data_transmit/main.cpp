#include "fnet_error.h"
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

    ret = eth.set_network("192.168.3.39","255.255.255.0","192.168.3.1"); // static interface
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

    // Open a socket on the network interface, and create a TCP connection to 192.168.4.1:4000
    
    socket.open(&eth);

    // eth.gethostbyname("raspberrypi.local", &a);
    a.set_ip_address("192.168.50.206");
    a.set_port(4000);
    socket.connect(a);
    if (ret != 0){
        tr_error("No IP address");
        goto end;
    }

    // Send a simple request
    for (int i=0; i<100000; i++){
        sprintf(sbuffer, "Hi, im client No.%d\r\n", i);
        int scount = socket.send(sbuffer, sizeof sbuffer);
        // printf("sent %d [%.*s]\n", scount, strstr(sbuffer, "\r\n") - sbuffer, sbuffer);
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

