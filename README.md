# Introduction
The Magic Mouse Pad is a remote control application that allows users to "infinitely" move their mouse by wrapping it in a window. This way, the mouse can travel a longer distance than the desktop size and thus control a cursor on a remote displays that is larger than the desktop.

# Getting Started
The Magic Mouse Pad comprises three projects: **mmpd** is the Magic Mouse Pad itself, which displays a window in which the mouse is caught until an escape key is pressed. The mouse pad application distributes mouse updates via a UDP socket. **mmpdcli** is a static client library that can be embedded into other applications to receive the updates from mmpd. Finally **mmouse** is a sample client application that receives the mouse updates from the mouse pad and moves the local mouse accordingly.

# Build
The Magic Mouse Pad comes in a Visual Studio 2019 solution. It should build and run out-of-the-box.

# Run
## mmpd

All configuration of mmpd is done via command line arguments. The following are supported:

`/ipversion [4|6]` – specifies the version of the IP to be used. This parameter defaults to 4.

`/port [port number]` – specifies the port to bind the server to. This parameter defaults to 47596.

`/bounds [left, top|left, top, right, bottom]` – specifies the overall bounds of the virtual display. If this parameter is not specified, the mouse can move indefinitely in each direction.

`/startpoint [x, y]` – specifies the initial virtual mouse position. If this parameter is not specified and the display is unbounded, this defaults to (0, 0). If the display is bounded, the start point is the centre of these bounds.

## mmouse

All configuration of mmouse is done via command line arguments. The following are supported:

`/port [port number]` – The port on which the application will receive mouse updates. If a server is specified, the application will announce this port to the server.

`/server [ip:port]` – The address of the mmpd server.
