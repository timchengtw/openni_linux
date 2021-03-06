#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#if defined(COMPRESS)
#include "zlib.h"
#endif

#if defined(RES_VGA)
#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#else
#define IMG_WIDTH 320
#define IMG_HEIGHT 240
#endif
#define IMG_FPS 30
#define SERVER_PORT 5566
#define CLIENT_PORT 5567
#define PKT_SIZE 51200
#define FID_SIZE 11
#if defined(COMPRESS)
#define PAYLOAD_SIZE 6
#define msleep(n) usleep(n*1000)
#endif

using namespace std;
using namespace xn;

int main( int argc, char* argv[] )
{

    if ( argc < 2 ) {
        cout << "No Client IP address" << endl;
        return 1;
    }

    // 1. Prepare OpenNI context and depth generator
    Context mContext;
    mContext.Init();

    XnMapOutputMode mapMode;
    mapMode.nXRes = IMG_WIDTH;
    mapMode.nYRes = IMG_HEIGHT;
    mapMode.nFPS = IMG_FPS;

    DepthGenerator mDepthGen;
    mDepthGen.Create( mContext );
    mDepthGen.SetMapOutputMode( mapMode );

    int socket_fd;
    int length;
    int nbytes;
    char buf[BUFSIZ];
    struct sockaddr_in myaddr;
    struct sockaddr_in client_addr;

    char frame_id[FID_SIZE] = {0};

#if !defined(COMPRESS)
    char *data1;
    char *data2;
    char *data3;
#if defined(RES_VGA)
    char *data4;
    char *data5;
    char *data6;
    char *data7;
    char *data8;
    char *data9;
    char *data10;
    char *data11;
    char *data12;
#endif
   
    data1 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
    data2 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
    data3 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
#if defined(RES_VGA)
    data4 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
    data5 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
    data6 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
    data7 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
    data8 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
    data9 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
    data10 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
    data11 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
    data12 = (char*)malloc((PKT_SIZE + FID_SIZE)*sizeof(char));
#endif
#else
    char *zData1;
    zData1 = (char*)malloc((PKT_SIZE + FID_SIZE + PAYLOAD_SIZE)*sizeof(char));
    char payload_size[PAYLOAD_SIZE] = {0};
#endif

    // 2. Prepare UDP socket 
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
        cout << "socket failed" << endl;
        return 1;
    }

    // 3. Set up address
    memset(&myaddr, 0, sizeof(myaddr));
    memset(&client_addr, 0, sizeof(client_addr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(SERVER_PORT);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(socket_fd, (struct sockaddr*)&myaddr, sizeof(myaddr)) < 0) {
        cout << "bind socket failed" << endl;
        return 1;
    }

    mContext.StartGeneratingAll();

    DepthMetaData mDepthMD;

    //while ( !xnOSWasKeyboardHit() )
    while ( true )
    {
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(CLIENT_PORT);
        inet_aton(argv[1], &client_addr.sin_addr);

        cout << "Received data from " << inet_ntoa(client_addr.sin_addr) << " : " << htons(client_addr.sin_port) << endl;
        //while ( !xnOSWasKeyboardHit() )
        while ( true )
        {
            mContext.WaitOneUpdateAll(mDepthGen);
            mDepthGen.GetMetaData(mDepthMD);
            sprintf(frame_id, "%d", mDepthMD.FrameID());
#if !defined(COMPRESS)
            *data1 = 'A';
            *data2 = 'B';
            *data3 = 'C';
#if defined(RES_VGA)
            *data4 = 'D';
            *data5 = 'E';
            *data6 = 'F';
            *data7 = 'G';
            *data8 = 'H';
            *data9 = 'I';
            *data10 = 'J';
            *data11 = 'K';
            *data12 = 'L';
#endif
            memcpy(data1 + 1, frame_id, FID_SIZE);
            memcpy(data1 + FID_SIZE, mDepthMD.Data(), PKT_SIZE);
            memcpy(data2 + 1, frame_id, FID_SIZE);
            memcpy(data2 + FID_SIZE, mDepthMD.Data() + PKT_SIZE/2, PKT_SIZE);
            memcpy(data3 + 1, frame_id, FID_SIZE);
            memcpy(data3 + FID_SIZE, mDepthMD.Data() + PKT_SIZE, PKT_SIZE);
#if defined(RES_VGA)
            memcpy(data4 + 1, frame_id, FID_SIZE);
            memcpy(data4 + FID_SIZE, mDepthMD.Data() + PKT_SIZE + PKT_SIZE/2, PKT_SIZE);
            memcpy(data5 + 1, frame_id, FID_SIZE);
            memcpy(data5 + FID_SIZE, mDepthMD.Data() + 2 * PKT_SIZE, PKT_SIZE);
            memcpy(data6 + 1, frame_id, FID_SIZE);
            memcpy(data6 + FID_SIZE, mDepthMD.Data() + 2 * PKT_SIZE + PKT_SIZE/2, PKT_SIZE);
            memcpy(data7 + 1, frame_id, FID_SIZE);
            memcpy(data7 + FID_SIZE, mDepthMD.Data() + 3 * PKT_SIZE, PKT_SIZE);
            memcpy(data8 + 1, frame_id, FID_SIZE);
            memcpy(data8 + FID_SIZE, mDepthMD.Data() + 3 * PKT_SIZE + PKT_SIZE/2, PKT_SIZE);
            memcpy(data9 + 1, frame_id, FID_SIZE);
            memcpy(data9 + FID_SIZE, mDepthMD.Data() + 4 * PKT_SIZE, PKT_SIZE);
            memcpy(data10 + 1, frame_id, FID_SIZE);
            memcpy(data10 + FID_SIZE , mDepthMD.Data() + 4 * PKT_SIZE + PKT_SIZE/2, PKT_SIZE);
            memcpy(data11 + 1, frame_id, FID_SIZE);
            memcpy(data11 + FID_SIZE, mDepthMD.Data() + 5 * PKT_SIZE, PKT_SIZE);
            memcpy(data12 + 1, frame_id, FID_SIZE);
            memcpy(data12 + FID_SIZE, mDepthMD.Data() + 5 * PKT_SIZE + PKT_SIZE/2, PKT_SIZE);
#endif
#else
            int err;
            Byte *zData;
            zData = (Byte*)calloc(IMG_WIDTH*IMG_HEIGHT*2, 1);
            uLong len = (uLong)(IMG_WIDTH*IMG_HEIGHT*2);
            uLong zDataLen = (uLong)(IMG_WIDTH*IMG_HEIGHT*2);
            err = compress2(zData, &zDataLen, (const Bytef*)mDepthMD.Data(), len, Z_BEST_COMPRESSION);
            if (err != Z_OK) {
                switch (err) {
                    case Z_ERRNO:
                        cout << "frame_id : " << frame_id << " compress error: Z_ERRNO" << endl;
                        break;
                    case Z_STREAM_ERROR:
                        cout << "frame_id : " << frame_id << " compress error: Z_STREAM_ERROR" << endl;
                        break;
                    case Z_DATA_ERROR:
                        cout << "frame_id : " << frame_id << " compress error: Z_DATA_ERROR" << endl;
                        break;
                    case Z_MEM_ERROR:
                        cout << "frame_id : " << frame_id << " compress error: Z_MEM_ERROR" << endl;
                        break;
                    case Z_BUF_ERROR:
                        cout << "frame_id : " << frame_id << " compress error: Z_BUF_ERROR" << endl;
                        break;
                    case Z_VERSION_ERROR:
                        cout << "frame_id : " << frame_id << " compress error: Z_VERSION_ERROR" << endl;
                        break;
                    default:
                        cout << "frame_id : " << frame_id << " compress error: " << err << endl;
                        break;
                }
                break;
            } else {
                cout << "frame_id : " << frame_id << ", zDataLen : " << zDataLen << ", mDepthMD.DataSize() : " << mDepthMD.DataSize() << endl;
                const char payload_offset[] = "ABCDEFGHIJKL";
                int index = 0;
                int add = 0;

                while(zDataLen > 0) {
                    memset(zData1, 0, PKT_SIZE + FID_SIZE + PAYLOAD_SIZE);
                    if (zDataLen > PKT_SIZE) {
                        sprintf(payload_size, "%d", PKT_SIZE);
                        memset(zData1, *(payload_offset + index), 1);
                        memcpy(zData1 + 1, frame_id, FID_SIZE);
                        memcpy(zData1 + FID_SIZE, &payload_size, PAYLOAD_SIZE);
                        memcpy(zData1 + FID_SIZE + PAYLOAD_SIZE, zData + add, PKT_SIZE);
                        zDataLen -= PKT_SIZE;
                        add += PKT_SIZE;
                    } else {
                        sprintf(payload_size, "%d", (int)zDataLen);
                        memset(zData1, *(payload_offset + index), 1);
                        memcpy(zData1 + 1, frame_id, FID_SIZE);
                        memcpy(zData1 + FID_SIZE, &payload_size, strlen(payload_size));
                        memcpy(zData1 + FID_SIZE + PAYLOAD_SIZE, zData + add, zDataLen);
                        zDataLen = 0;
                    }
                    index++;
                    sendto( socket_fd, (char*)zData1, PKT_SIZE + FID_SIZE + PAYLOAD_SIZE, 0, (struct sockaddr*)&client_addr, length);
                    msleep(30);
                }
                add = 0;
            }
            std::free(zData);
#endif
            length = sizeof(client_addr);
#if !defined(COMPRESS)
            sendto( socket_fd, data1, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
            sendto( socket_fd, data2, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
            sendto( socket_fd, data3, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
#if defined(RES_VGA)
            sendto( socket_fd, data4, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
            sendto( socket_fd, data5, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
            sendto( socket_fd, data6, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
            sendto( socket_fd, data7, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
            sendto( socket_fd, data8, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
            sendto( socket_fd, data9, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
            sendto( socket_fd, data10, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
            sendto( socket_fd, data11, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
            sendto( socket_fd, data12, PKT_SIZE + FID_SIZE, 0, (struct sockaddr*)&client_addr, length);
#endif
#endif


        }
    }

    mContext.StopGeneratingAll();

    mContext.Release();

#if !defined(COMPRESS)
    free(data1);
    free(data2);
    free(data3);
#if defined(RES_VGA)
    free(data4);
    free(data5);
    free(data6);
    free(data7);
    free(data8);
    free(data9);
    free(data10);
    free(data11);
    free(data12);
#endif
#else
    free(zData1);
#endif
    return 0;
}

