// Server.cpp
#include <SDL_net.h>
#include <iostream>

const int PORT = 12345; // Port number to listen on

int main(int argc, char* args[]) {
    if (SDLNet_Init() < 0) {
        std::cerr << "SDLNet_Init() failed: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    IPaddress ip;
    TCPsocket serverSocket;

    if (SDLNet_ResolveHost(&ip, NULL, PORT) == -1) {
        std::cerr << "SDLNet_ResolveHost() failed: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return 1;
    }

    serverSocket = SDLNet_TCP_Open(&ip);

    if (!serverSocket) {
        std::cerr << "SDLNet_TCP_Open() failed: " << SDLNet_GetError() << std::endl;
        SDLNet_Quit();
        return 1;
    }

    // Server logic here

    SDLNet_TCP_Close(serverSocket);
    SDLNet_Quit();
    return 0;
}