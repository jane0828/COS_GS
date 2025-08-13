#include <utl/utl.h>
#include <ifc/ifc.h>

SGS_RS232::SGS_RS232(const char *devname, uint32_t baud) {
    fd = open(devname, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        std::cerr << "Error opening serial port" << std::endl;
        return;
    }
    fcntl(fd, F_SETFL, 0);
    tcgetattr(fd, &tio);
    SetBaudRate(baud);
}

SGS_RS232::~SGS_RS232() {
    if (fd != -1) {
        close(fd);
    }
}

void SGS_RS232::SetBaudRate(uint32_t baud) {
    speed_t speed;
    switch (baud) {
        case 9600: speed = B9600; break;
        case 19200: speed = B19200; break;
        case 38400: speed = B38400; break;
        case 57600: speed = B57600; break;
        case 115200: speed = B115200; break;
        default:
            std::cerr << "Unsupported baud rate" << std::endl;
            return;
    }
    cfsetispeed(&tio, speed);
    cfsetospeed(&tio, speed);
    tio.c_cflag |= (CLOCAL | CREAD);
    tio.c_cflag &= ~PARENB;
    tio.c_cflag &= ~CSTOPB;
    tio.c_cflag &= ~CSIZE;
    tio.c_cflag |= CS8;
    tcsetattr(fd, TCSANOW, &tio);
}

ssize_t SGS_RS232::Write(const void *data, size_t size) {
    tcflush(this->fd, TCIOFLUSH);
    return write(fd, data, size);
}

ssize_t SGS_RS232::Read(void *buffer, size_t size) {
    return read(fd, buffer, size);
}

void SGS_RS232::Close() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}