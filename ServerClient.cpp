#include "ServerClient.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Message.h"

ServerClient::ServerClient() :
    _socket ( INVALID_SOCKET )
{
}  

ServerClient::~ServerClient() {
    closesocket(_socket);
    //delete[] something 
}

bool ServerClient::connect(SOCKET listen_socket) {
    _socket = accept(listen_socket, NULL, NULL);
    if(_socket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        return false;
    }

    printf("New Client Accepted\n");
    return true;
}

bool ServerClient::associate(HANDLE iocp) {
    HANDLE handle = CreateIoCompletionPort((HANDLE)_socket, iocp, (DWORD)_socket, 0);
    if(handle == NULL) {
        printf("Client CreateIOCompletionPort failed %lu: \n", GetLastError());
        return false;
    }
    return true;
}

void ServerClient::send(Message* message) {
    ZeroMemory(&_send_context._overlapped, sizeof(WSAOVERLAPPED));
    memset(_send_context._buffer, 0, DATA_BUFFER_SIZE);
    _send_context._client = this;
    _send_context._operation = OP_WRITE;
    WSABUF buf;
    buf.len = message->_size;
    buf.buf = _send_context._buffer;
    DWORD bytes_sent = 0;
    DWORD flags = 0;

    char* data = message->data();
    memcpy(_send_context._buffer, data, message->_size);
    delete[] data;

    int send_ret = WSASend(_socket, &buf, 1, &bytes_sent, flags, (OVERLAPPED*) &_send_context, NULL);
    if(send_ret == SOCKET_ERROR) {
        printf("WSASend failed: %d\n", WSAGetLastError());
    }
    else {
        printf("WSASend returned immedietly\n");
    }
}

void ServerClient::receive() {
    ZeroMemory(&_receive_context._overlapped, sizeof(WSAOVERLAPPED));
    memset(_receive_context._buffer, 0, DATA_BUFFER_SIZE);
    _receive_context._client = this;
    _receive_context._operation = OP_READ;
    WSABUF buf;
    buf.len = DATA_BUFFER_SIZE;
    buf.buf = _receive_context._buffer;
    DWORD bytes_recieved = 0;
    DWORD flags = 0;
    
    int recv_ret = WSARecv(_socket, &buf, 1, &bytes_recieved, &flags, (OVERLAPPED*)&_receive_context, NULL);
    if(recv_ret == SOCKET_ERROR) {
        printf("WSARecv failed: %d\n", WSAGetLastError());
    }
    else {
        printf("WSARecv returned immedietly\n");
    }
}
