# Magic Mouse Pad
The Magic Mouse Pad is a C++ application that allows for using pixel-perfect mouse operations on the VVand. The mouse pad application is a window that captures the mouse and resets it when leaving its client are such that it can be moved idefinitely. The client library allows applications to receive mouse events from the mouse pad application via UDP.

## Getting started
The project comprises three components: [mmpcli](mmpcli) is the client library, which is also used by the server for certain functions. Applications interested in mouse events should link against this library to connect to an instance of the server.

[magicmousepad](magicmousepad) is this server component. It shows a blank window that captures the mouse on click and notifies all registered listeners about movements. The capture of the mouse pad can be released by pressing the **Pause** key.

Finally [magicmouse](magicmouse) is a demo tool that moves the mouse around based on the data received from the server application.
