Example Summary
---------------
This application demonstrates how to use TCP with TLS.

Peripherals Exercised
---------------------
Board_LED0      Indicates that the board was initialized within main()
Board_EMAC      Connection to network

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| DK-TM4C129X       | N/A                                                    |
| EK-TM4C1294XL     |                                                        |
| ----------------- | ------------------------------------------------------ |

Build Details
-------------
Before building the example,
  Add '<CyaSSL root directory>' path to the compiler include path.
  Add '<CyaSSL root directory>/tirtos/packages/ti/net/cyassl/lib/ti.net.cyassl.a<target>' to the linker options.
  NOTE: Make sure the library ordering is correct on the linker command line
  to avoid linker errors.

  Wallclock time support has been added in the application code. This time
  is needed by CyaSSL libraries to validate certificates. Make sure the time
  is set to the current time (in secs) in the application code (in main()).

Example Usage
-------------
The device must be connected to a network with a DHCP server to run this example
successfully.

The example turns ON Board_LED0 and starts the CyaSSL stack and the network
stack. When the stack receives an IP address from the DHCP server,
the IP address is written to the console.

To test the example, there are two client tool options we suggest:
Download and run the socat Linux or Windows tool available on internet.

  Usage:  socat stdio openssl-connect:<IP-addr>:<port>,cafile=<CyaSSL-dir>/certs/ca-cert.pem

  <IP-addr>    is the IP address of the application.
  <port>       is the TCP port to being listened to (1000).
  <CyaSSL-dir> is the path to the CyaSSL directory.

  Enter the data to be sent to the tcpEchoTLS server and see the data
  echoed from the tcpEchoTLS server on the screen.

Or, run the CyaSSL echoclient executable to connect to the tcpEchoServer
application running on the target board. The executable can built from the
sources available on CyaSSL manual webpage. Look for Chapter 11 - SSL Tutorial
in the CyaSSL manual for detailed build instructions:
  http://www.yassl.com/yaSSL/Docs-cyassl-manual-11-ssl-tutorial.html

  Usage: ./echoclient <IP-addr>

  <IP-addr> is the IP address of the application.

  Make sure the echoclient example is using the correct port (1000).

  Enter the data to be sent to the tcpEchoTLS server and see the data
  echoed from the tcpEchoTLS server on the screen.

Application Design Details
--------------------------
This application uses two types of task:

tcpHandler  Creates a new TLS context, loads certificates, creates a socket and
            accepts incoming connections.  When a connection is established
            a tcpWorker task is dynamically created to send or receive
            data securely.
tcpWorker   Echoes TCP packages back to the client securely.

  'tcpHandler' performs the following actions:
      Create a new TLS context, loads the certificate authority and server
      certificates, and server key to the context buffers.

      Create a socket and bind it to a port (1000 for this example).

      Wait for incoming requests (default: max 3 requests).

      Once a request is received, the request is bound to a TLS instance
      and a new tcpWorker task is dynamically created to manage the
      the communication (echo encrypted TCP packets).

      Waiting for new requests.

  'tcpWorker' performs the following actions:
      Allocate memory to serve as a TCP packet buffer.

      (Calls CyaSSL_read and CyaSSL_write API to decrypt and encrypt
      the communication with the client).

      Receive data from socket client (Data decrypted by TLS layer before it
      is accessible by the user application).

      Echo back TCP packet to the client (Data encrypted by TLS layer before
      sending to the client).

      When client closes the socket, free TLS instance, close server socket,
      free TCP buffer memory and exit the task.

This example can be compared to the tcpEcho example to see the TLS layer that
is added to make the TCP communication secure.

For GNU and IAR users, please read the following website for details about semi-hosting
http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting.
