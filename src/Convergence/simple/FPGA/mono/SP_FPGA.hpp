#ifndef _SP_NEON_OMP_
#define _SP_NEON_OMP_

#include "Convergence/Convergence.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class SP_FPGA : public Convergence {
protected :
    int sockfd;
    int portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    bool connected;

public:

    SP_FPGA();

    SP_FPGA(ColorMap *_colors, int _max_iters);

    ~SP_FPGA();

    virtual void updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr);

    virtual bool is_valid();

};

#endif
