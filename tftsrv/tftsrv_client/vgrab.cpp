
/* 
 * vg_* functions are derived from: 
 *
V4L2 video picture grabber
Copyright (C) 2009 Mauro Carvalho Chehab <mchehab@infradead.org>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
 *
 * and as such this file is under GPLV2
 *
 */

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <libv4l2.h>

char *_vg_b = nullptr;
size_t vg_n = 0;
uint32_t vg_w = 0, vg_h = 0;
int vg_dev = -1;

void error (const char *s1) {
    fprintf (stderr, "%s\n", s1);
}
void error (const char *s1, const char *s2) {
    fprintf (stderr, "%s %s\n", s1, s2);
}

bool vg_ioctl (int f, int req, void *arg) {
    int res_i;

    do {
        res_i = v4l2_ioctl (f, req, arg);
    } while (res_i == -1 && ((errno == EINTR) || (errno == EAGAIN)));

    if (res_i == -1) {
        error ("vg_ioctl", strerror (errno));
        return false;
    }
    return true;
}

bool vg_open (const char *dev_s, uint32_t w, uint32_t h) {
    struct v4l2_format fmt;
    struct v4l2_buffer buf;
    struct v4l2_requestbuffers req;

    vg_dev = v4l2_open (dev_s, O_RDWR | O_NONBLOCK, 0);
    if (vg_dev < 0) {
        error ("vg_open: open: ", dev_s);
        return false;
    }

    memset (&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = w;
    fmt.fmt.pix.height = h;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    if (! vg_ioctl (vg_dev, VIDIOC_S_FMT, &fmt)) {
        error ("vg_open: vg_ioctl VIDIOC_S_FMT failed");
        return false;
    }
    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24) {
        error ("vg_open: pixelformat is not V4L2_PIX_FMT_RGB24");
        return false;
    }
    else if ((fmt.fmt.pix.width != w) || (fmt.fmt.pix.height != h)) {
        if (fmt.fmt.pix.width != w)
            error ("vg_open: not requested x resolution");
        if (fmt.fmt.pix.height != h)
            error ("vg_open: not requested y resolution");
        return false;
    }

    memset (&req, 0, sizeof(req));
    req.count = 1;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    if (! vg_ioctl (vg_dev, VIDIOC_REQBUFS, &req)) {
        error ("vg_open: vg_ioctl VIDIOC_REQBUFS failed");
        return false;
    }

    memset (&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    //buf.index = 0;
    if (! vg_ioctl (vg_dev, VIDIOC_QUERYBUF, &buf)) {
        error ("vg_open: vg_ioctl VIDIOC_QUERYBUF failed");
        return false;
    }

    vg_n = buf.length;
    _vg_b = (char *)v4l2_mmap (nullptr, buf.length,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        vg_dev, buf.m.offset);

    if (_vg_b == MAP_FAILED) {
        error ("vg_open: mmap failed");
        return false;
    }
 
    vg_w = w;
    vg_h = h;
    return true;
}

bool vg_grab () {
    struct v4l2_buffer buf;
    enum v4l2_buf_type type;
    fd_set fds;
    struct timeval tv;
    int res_i;

    memset (&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    //buf.index = 0;
    if (! vg_ioctl (vg_dev, VIDIOC_QBUF, &buf)) {
        error ("vg_grab: vg_ioctl VIDIOC_QBUF failed");
        return false;
    }

    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (! vg_ioctl (vg_dev, VIDIOC_STREAMON, &type)) {
        error ("vg_grab: vg_ioctl VIDIOC_STREAMMON failed");
        return false;
    }
    
    do {
        FD_ZERO (&fds);
        FD_SET (vg_dev, &fds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        res_i = select (vg_dev + 1, &fds, nullptr, nullptr, &tv);
    } while ((res_i == -1 && (errno = EINTR)));

    if (res_i == -1) {
        error ("vg_grab: select");
        return false;
    }

    memset (&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (! vg_ioctl (vg_dev, VIDIOC_DQBUF, &buf)) {
        error ("vg_grab: vg_ioctl failed");
        return false;
    }

    if (! vg_ioctl (vg_dev, VIDIOC_QBUF, &buf)) {
        error ("vg_grab: vg_ioctl VIDIOC_QBUF failed");
        return false;
    }
    
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (! vg_ioctl (vg_dev, VIDIOC_STREAMOFF, &type)) {
        error ("vg_grab: vg_ioctl VIDIOC_STREAMOFF failed");
        return false;
    }

    return true;
}

void vg_close () {
    v4l2_munmap (_vg_b, vg_n);
    v4l2_close (vg_dev);
}

bool vg_save_ppm (const char *file_s) {
    FILE *f_out;

    f_out = fopen (file_s, "wb");
    if (! f_out) {
        error ("vg_save_ppm: can not create: ", file_s);
        return false;
    }
    fprintf (f_out, "P6\n%d %d 255\n", vg_w, vg_h);
    fwrite (_vg_b, vg_n, 1, f_out);
    fclose (f_out);
    return true;
}

int main (int argc, char **argv) {
    if (argc < 2) {
        fprintf (stderr, "usage: %s output_file [video dev]\n", argv[0]);
        return -1;
    }
    
    if (argc == 3) {
        if (! vg_open (argv[2], 320, 240))
            return -2;
    }
    else if (! vg_open ("/dev/video0", 320, 240))
        return -3;

    if (! vg_grab ())
        return -4;

    if (! vg_save_ppm (argv[1]))
        return -5;

    return 0;
}

