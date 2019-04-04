#pragma once
#include "EventLoop.h"

int socket_bind_listen(int port);

void handle_for_sigpipe();

int setSocketNonBlocking(int fd);

void httpinit(EventLoop* loop);

ssize_t writen(int fd, void *buff, size_t n);

ssize_t writen(int fd, std::string &sbuff);

ssize_t readn(int fd, std::string &inBuffer, bool &zero);