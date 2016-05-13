Example Summary
---------------
This application demonstrates how to use TCP over a PPP connection. The
connection that is made is a direct serial connection via USB cable.

Please also refer to the following documents for more information on PPP, HDLC,
llSerial, and NDK mini drivers:

    - NDK Reference Guide (spru524i.pdf or latest version)
    - NDK User's Guide (spru523i.pdf or latest version)
    - NSP Ethernet Driver Design Guide (sprufp2_nspethdrvdesign.pdf)
    - TCP/IP NDK Platform Porting Guide (port303.pdf, includes further details
      of llSerial and serial mini driver interface (HwSer* functions))

Peripherals Exercised
---------------------
Board_LED0      Indicates that the board was initialized within main()
Board_USBDEVICE Used for serial communication*

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| DK-TM4C129X       | Please ensure that the board's USB controller is       |
| EK-TM4C1294XL     | connected to your host via a USB cable. Note the USB   |
|                   | connection needed for serial/PPP communication is      |
|                   | *in addition* to the USB connection that's connected   |
|                   | to the board for emulation.                            |
|                   |                                                        |
|                   | A VCOM (virtual COM) port driver may need to be        |
|                   | installed.                                             |
| ----------------- | ------------------------------------------------------ |

Example Usage
-------------
The device must be connected via USB cable to a PC running the Linux OS in
order to run successfully.

The example turns ON Board_LED0 and starts the network stack. No IP address
information will be displayed until a successful connection to a PPP server
has been established.

Special Build Steps
-------------------
Note that the following constant must be defined with value 1 when building
this example:

    TI_EXAMPLES_PPP

This constant is defined and set as a compiler flag in the example, however any
separate application that builds off of this one must remember to do this in
order to get the correct definition of the NIMU device table in board.c.

Connect to a PPP server running on the PC:
------------------------------------------
Linux:

Use the 'pppd' command to start a PPP server which awaits connection from a
PPP client (in this case the host running TI-RTOS).  The following Linux
command will start such a server and assign the local IP address of 10.90.90.2
to the PPP interface and 10.90.90.4 to the PPP interface of the embedded device.
Note that the below command assumes the USB connection has been made on device
"/dev/ttyACM0" (you can check the actual device by typing "ls /dev/tty*" on
Linux):

    sudo pppd -detach crtscts noaccomp noccp nopcomp lock defaultroute silent \
        -d kdebug 2 receive-all lcp-max-configure 3 10.90.90.2:10.90.90.4 \
        /dev/ttyACM0 115200

    Note: It was observed during testing that some Linux PCs have an
    application called "modem manager" running, which can conflict with the
    NDK's PPP connection and prevent the 'pppd' command from executing
    successfully.  If you experience problems connecting with the 'pppd'
    command, or see error messages due to receiving a "Bad Packet" on the
    target side, it may be due to a conflict caused by the "modem manager"
    process.  In this event, it is necessary to disable or possibly even
    remove the "modem manager" application from the Linux PC.

Upon successful connection, you should see output similar to the following:

    using channel 221
    Using interface ppp0
    Connect: ppp0 <--> /dev/ttyACM0
    rcvd [LCP ConfReq id=0x2 <mru 1500> <asyncmap 0x0> <magic 0x0>]
    sent [LCP ConfReq id=0x1 <asyncmap 0x0> <magic 0xb6255260>]
    sent [LCP ConfAck id=0x2 <mru 1500> <asyncmap 0x0> <magic 0x0>]
    rcvd [LCP ConfAck id=0x1 <asyncmap 0x0> <magic 0xb6255260>]
    sent [LCP EchoReq id=0x0 magic=0xb6255260]
    sent [IPCP ConfReq id=0x1 <compress VJ 0f 01> <addr 10.90.90.2>]
    rcvd [IPCP ConfReq id=0x1 <addr 0.0.0.0> <ms-dns1 0.0.0.0> <ms-wins 0.0.0.0> <ms-dns2 0.0.0.0> <ms-wins 0.0.0.0>]
    sent [IPCP ConfRej id=0x1 <ms-dns1 0.0.0.0> <ms-wins 0.0.0.0> <ms-dns2 0.0.0.0> <ms-wins 0.0.0.0>]
    rcvd [LCP EchoRep id=0x0 magic=0x0]
    rcvd [IPCP ConfRej id=0x1 <compress VJ 0f 01>]
    sent [IPCP ConfReq id=0x2 <addr 10.90.90.2>]
    rcvd [IPCP ConfReq id=0x2 <addr 0.0.0.0>]
    sent [IPCP ConfNak id=0x2 <addr 10.90.90.4>]
    rcvd [IPCP ConfAck id=0x2 <addr 10.90.90.2>]
    rcvd [IPCP ConfReq id=0x3 <addr 10.90.90.4>]
    sent [IPCP ConfAck id=0x3 <addr 10.90.90.4>]
    not replacing existing default route via 146.252.162.1
    local  IP address 10.90.90.2
    remote IP address 10.90.90.4
    Script /etc/ppp/ip-up started (pid 25239)
    Script /etc/ppp/ip-up finished (pid 25239), status = 0x0

You should also see The IP address of '10.90.90.4' written in the console in
CCS.

Use the 'ifconfig' command on Linux to see that the ppp0 interface was added:

    ppp0  Link encap:Point-to-Point Protocol
          inet addr:10.90.90.2  P-t-P:10.90.90.4  Mask:255.255.255.255
          UP POINTOPOINT RUNNING NOARP MULTICAST  MTU:1500  Metric:1
          RX packets:5 errors:0 dropped:0 overruns:0 frame:0
          TX packets:5 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:3
          RX bytes:74 (74.0 B)  TX bytes:74 (74.0 B)

Run the tcpSendReceive Linux or Windows executable that is shipped with TI-RTOS.
The executable is found in:

  <tirtos_install_dir>\packages\examples\tools\tcpSendReceive

  Usage: ./tcpSendReceive <IP-addr> <port> <id> -l[length] -s[sleep in uS]

  <IP-addr> is the IP address
  <port>    is the TCP port being listened to (1000)
  <id>      is a unique id for the executable. Printed out when 1000 packets are
            transmitted. It allows the user to run multiple instances
            of tcpSendReceive.

  Optional:
    -l[length]      size of the packet in bytes. Default is 1024 bytes.
    -s[sleep in uS] usleep time to between sends. Default is 1000 uSecs.

  Example:
        tcpSendReceive 10.90.90.4 1000 1 -s100

Messages such as the following will begin to appear on the terminal window when
a TCP packet has been echoed back:

        Starting test with a 1000 uSec delay between transmits
        [id 1] count = 1000, time = 12
        [id 1] count = 2000, time = 24
        [id 1] count = 3000, time = 36

Application Design Details
--------------------------
This example implements the NDK's llSerial (low level serial) and serial mini
driver interfaces.  The llSerial implementation is a hardware independent
layer and uses HDLC mode only (note character mode was omitted for this
example). This can be found in the llserial.h and llserial.c files.

The serial mini driver implementation, which is typically a hardware dependent
API set, can be found in the usbserialppp.c file.  In this case it does not
contain any hardware dependent code, rather it calls into the USBCDCD API set
found in the USB serial device example files (USBCDCD.h and USBCDCD.c, copies
of which are included with this example).  The USBCDCD APIs are used to
send and receive (PPP) data over the direct serial connection via USB cable to
a PC.

The following task is used to recieve data on the USB connection:

  spIsrTask   Task used in place of an ISR to receive data.  Since USBCDCD does
              not have any interrupt that triggers when data comes in on the
              wire, a Task is used to mimic ISR behavior.  Signals back to stack
              once a full HDLC frame has been received.

The following function is used to send data on the USB connection. No interrupt
is required for the transmission of data; therefore this function runs within
the context of the NDK stack thread when appropriate.

  spWriteFifo Handles HDLC framing and CRC calculations are handled here.

The following tasks are used to handle TCP communications:

  tcpHandler  Creates a socket and accepts incoming connections.  When a
              connection is established a tcpWorker task is dynamically
              created to send or receive data.
  tcpWorker   Echoes TCP packages back to the client.

  'tcpHandler' performs the following actions:
      Create a socket and bind it to a port (1000 for this example).

      Wait for incoming requests.

      Once a request is received, a new tcpWorker task is dynamically created to
      manage the communication (echo TCP packets).

      Waiting for new requests.

  'tcpWorker' performs the following actions:
      Allocate memory to serve as a TCP packet buffer.

      Receive data from socket client.

      Echo the TCP packet back to the client.

      When client closes the socket, close server socket, free TCP buffer memory
      and exit the task.

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting
