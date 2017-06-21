#include "videodevice.h"

VideoDevice::VideoDevice() {
    this->dev_name = new char[12];
    strcpy(dev_name, "/dev/video0");
    this->fd = -1;
    this->buffers = NULL;
    this->n_buffers = 0;
    this->index = -1;
}

int VideoDevice::open_device() {
    for (int i = 0; -1 == fd && i < 10; i++) {
        dev_name[10] = '0' + i;
        fd = open(dev_name, O_RDWR, 0);
    }
    if (-1 == fd) {
        qFatal("open: %s", strerror(errno));
    } else {
        qDebug("use video device: %s", dev_name);
    }
    return 0;
}

int VideoDevice::close_device() {
    if(-1 == close(fd)) {
        qWarning("close: %s", strerror(errno));
    }
    return 0;
}

int VideoDevice::init_device() {
    v4l2_capability cap;
    v4l2_cropcap cropcap;
    v4l2_crop crop;
    v4l2_format fmt;

    if(-1 == ioctl(fd, VIDIOC_QUERYCAP, &cap)) {
        if(EINVAL == errno) {
            qWarning("%s is no V4l2 device", dev_name);
        } else {
            qWarning("VIDIOC_QUERYCAP: %s", strerror(errno));
        }
    }

    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        qWarning("%s is no video capture device", dev_name);
    }

    if(!(cap.capabilities & V4L2_CAP_STREAMING)) {
        qWarning("%s does not support streaming i/o", dev_name);
    }

    CLEAR(cropcap);

    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(0 == ioctl(fd, VIDIOC_CROPCAP, &cropcap)) {
        CLEAR(crop);
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect;

        if(-1 == ioctl(fd, VIDIOC_S_CROP, &crop)) {
            if(EINVAL == errno)  {
                qWarning("VIDIOC_S_CROP not supported");
            } else {
                qWarning("VIDIOC_S_CROP: %s", strerror(errno));
            }
        }
    } else {
        qWarning("VIDIOC_CROPCAP: %s", strerror(errno));
    }

    CLEAR(fmt);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = Width;
    fmt.fmt.pix.height = Height;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;

    if(-1 == ioctl(fd, VIDIOC_S_FMT, &fmt)) {
        qWarning("VIDIOC_S_FMT: %s", strerror(errno)) ;
    }

    return init_mmap();
}

int VideoDevice::init_mmap() {
    v4l2_requestbuffers req;
    CLEAR(req);

    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if(-1 == ioctl(fd, VIDIOC_REQBUFS, &req)) {
        if(EINVAL == errno) {
            qWarning("%s does not support memory mapping", dev_name);
        } else {
            qWarning("VIDIOC_REQBUFS %s", strerror(errno));
        }
    }

    if(req.count < 2) {
        qWarning("Insufficient buffer memory on %s", dev_name);
    }

    buffers = (buffer*)calloc(req.count, sizeof(*buffers));

    if(!buffers) {
        qWarning("out of memory");
    }

    for(n_buffers = 0; n_buffers < req.count; ++n_buffers) {
        v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = n_buffers;

        if(-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf)) {
            qWarning("VIDIOC_QUERYBUF: %s", strerror(errno));
        }

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start =
                mmap(NULL, // start anywhere
                     buf.length,
                     PROT_READ | PROT_WRITE,
                     MAP_SHARED,
                     fd, buf.m.offset);

        if(MAP_FAILED == buffers[n_buffers].start) {
            qWarning("mmap: %s", strerror(errno));
        }
    }
    return 0;

}

int VideoDevice::start_capturing() {
    unsigned int i;
    for(i = 0; i < n_buffers; ++i) {
        v4l2_buffer buf;
        CLEAR(buf);

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory =V4L2_MEMORY_MMAP;
        buf.index = i;

        if(-1 == ioctl(fd, VIDIOC_QBUF, &buf)) {
            qWarning("VIDIOC_QBUF: %s", strerror(errno));
        }
    }

    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(-1 == ioctl(fd, VIDIOC_STREAMON, &type)) {
        qWarning("VIDIOC_STREAMON: %s", strerror(errno));
    }
    return 0;
}

int VideoDevice::stop_capturing() {
    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if(-1 == ioctl(fd, VIDIOC_STREAMOFF, &type)) {
        qWarning("VIDIOC_STREAMOFF: %s", strerror(errno));
    }
    return 0;
}

int VideoDevice::uninit_device()
{
    unsigned int i;
    for(i = 0; i < n_buffers; ++i) {
        if(-1 == munmap(buffers[i].start, buffers[i].length)) {
            qWarning("munmap: %s", strerror(errno));
        }
    }
    free(buffers);
    return 0;
}

int VideoDevice::get_frame(void **frame_buf, unsigned * len) {
    v4l2_buffer queue_buf;
    CLEAR(queue_buf);

    queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    queue_buf.memory = V4L2_MEMORY_MMAP;

    if(-1 == ioctl(fd, VIDIOC_DQBUF, &queue_buf)) {
        qWarning("VIDIOC_DQBUF: %s", strerror(errno));
    }

    *frame_buf = buffers[queue_buf.index].start;
    *len = buffers[queue_buf.index].length;
    index = queue_buf.index;

    return 0;
}

int VideoDevice::unget_frame() {
    if(index != -1) {
        v4l2_buffer queue_buf;
        CLEAR(queue_buf);

        queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        queue_buf.memory = V4L2_MEMORY_MMAP;
        queue_buf.index = index;

        if(-1 == ioctl(fd, VIDIOC_QBUF, &queue_buf)) {
            qWarning("VIDIOC_QBUF: %s", strerror(errno));
        }
        return 0;
    }
    return -1;
}
