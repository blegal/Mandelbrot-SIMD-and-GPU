#include "SP_FPGA.hpp"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

SP_FPGA::SP_FPGA() : Convergence("SP_FPGA")
{
    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "FPGA";
    modeOPENMP  = "disable";
    OTHER       = "none";

    portno = 57345;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname( "192.168.1.15" );
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);

    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

}

SP_FPGA::SP_FPGA(ColorMap *_colors, int _max_iters) : Convergence("SP_FPGA")
{
    colors      = _colors;
    max_iters   = _max_iters;

    fractal     = "mandelbrot";
    dataFormat  = "float";
    modeSIMD    = "FPGA";
    modeOPENMP  = "disable";
    OTHER       = "none";
    connected   = true;

    portno = 57345;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        connected = false;
        return;
//        error("ERROR opening socket");
    }

    struct timeval timeout;
    timeout.tv_sec  = 1;
    timeout.tv_usec = 0;

    if (setsockopt (sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                    sizeof(timeout)) < 0)
        error("setsockopt failed\n");

    if (setsockopt (sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                    sizeof(timeout)) < 0)
        error("setsockopt failed\n");

    server = gethostbyname( "192.168.1.15" );
    if (server == NULL)
    {
        connected = false;
        return;
//        fprintf(stderr,"ERROR, no such host\n");
//        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);

    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        connected = false;
        return;
//        error("ERROR connecting");
    }
}

SP_FPGA::~SP_FPGA()
{

}


void SP_FPGA::updateImage(const long double _zoom, const long double _offsetX, const long double _offsetY, const int IMAGE_WIDTH, const int IMAGE_HEIGHT, float* ptr)
{
    const float f_zoom    = (float)_zoom;
    const float f_offsetX = (float)_offsetX;
    const float f_offsetY = (float)_offsetY;

    const float Xstart = f_offsetX - IMAGE_WIDTH  / 2.0f * f_zoom;
    const float Ystart = f_offsetY - IMAGE_HEIGHT / 2.0f * f_zoom;

    if( connected == false )
    {
        return;
    }

    uint32_t bSend;
#if 0
    bSend = write(sockfd, &Xstart, sizeof(char));
    assert( bSend == sizeof(char) );
#else
    bSend = write(sockfd, &Xstart,        sizeof(float));
    assert( bSend == sizeof(float) );
    bSend = write(sockfd, &Ystart,        sizeof(float));
    assert( bSend == sizeof(float) );
    bSend = write(sockfd, &f_zoom,        sizeof(float));
    assert( bSend == sizeof(float) );
    bSend = write(sockfd, &IMAGE_WIDTH,   sizeof(int));
    assert( bSend == sizeof(int) );
    bSend = write(sockfd, &IMAGE_HEIGHT,  sizeof(int));
    assert( bSend == sizeof(int) );
    bSend = write(sockfd, &max_iters, sizeof(int));
    assert( bSend == sizeof(int) );
#endif

    const uint32_t bytes = IMAGE_WIDTH * IMAGE_HEIGHT;
    uint8_t* ubuffer = new uint8_t[bytes];

    uint32_t nRcv = 0;
    while( nRcv != (sizeof(uint8_t)*bytes) )
    {
        const int32_t r = read(sockfd, ubuffer + nRcv, sizeof(uint8_t) * bytes - nRcv);
        if( r <= 0 )
        {
            printf("(WW) Warning the data transfer seems blocked...\n");
            break;
        }
        nRcv += r;
    }

    for(uint32_t i = 0; i < bytes; i += 1)
        ptr[ i ] = 0;

    for(uint32_t i = 0; i < nRcv; i += 1)
        ptr[ i ] = ubuffer[ i ];

    delete[] ubuffer;

}


bool SP_FPGA::is_valid()
{
    return connected;
}
