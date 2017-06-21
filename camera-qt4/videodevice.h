#ifndef VIDEODEVICE_H
#define VIDEODEVICE_H

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


#include <sys/ioctl.h>
#include <sys/mman.h>

#include <asm/types.h>
#include <linux/videodev2.h>

#include <QString>
#include <QObject>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define Width 640
#define Height 480

class VideoDevice : public QObject {
    Q_OBJECT
public:
    VideoDevice();
    int open_device();
    int close_device();
    int init_device();
    int start_capturing();
    int stop_capturing();
    int uninit_device();
    int get_frame(void **, unsigned *);
    int unget_frame();

private:
    int init_mmap();

    struct buffer {
        void * start;
        unsigned  length;
    };
    char* dev_name;
    int fd;
    buffer* buffers;
    unsigned int n_buffers;
    int index;

};

#endif // VIDEODEVICE_H
