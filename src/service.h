#pragma once

#ifndef SERVICE_H
#define SERVICE_H

#include "ipcam-socket-server.h"

class IpcamSession
{
public:
    IpcamSession(io_service& io_service, string ipcam_name, string ipcam_url);
    ~IpcamSession();
    tcp::socket& socket();
    void do_work();

private:
    string get_photo();
    string get_status();

    tcp::socket socket_;
    io_service& io_service_;
    string ipcam_url_;
    string ipcam_name_;
};

class SocketServer
{
public:
    SocketServer(io_service& io_service, string ipcam_url, string ipcam_name, int sock);
    ~SocketServer();
private:
    void start_accept();
    void handle_accept(IpcamSession* new_session, const boost::system::error_code& error);

    io_service& io_service_;
    tcp::acceptor acceptor_;
    string ipcam_url_;
    string ipcam_name_;
    interprocess_semaphore sem_;
};

#endif
