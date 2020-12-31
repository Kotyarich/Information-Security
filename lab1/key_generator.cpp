#include "key_generator.h"

#include <linux/hdreg.h>
#include <sys/ioctl.h>
#include <fcntl.h>

std::string getLinuxInfo() {
    static struct hd_driveid hd;
    int fd;

    if ((fd = open("/dev/sda", O_RDONLY | O_NONBLOCK)) < 0) {
        printf("ERROR opening /dev/sda\n");
    }

    if (!ioctl(fd, HDIO_GET_IDENTITY, &hd)) {
        char buf[21];
        snprintf(buf, 20, "%s", hd.serial_no);
        auto serial_number = std::string(buf);
        size_t startpos = serial_number.find_first_not_of(" \t");
        if (std::string::npos != startpos) {
            serial_number = serial_number.substr(startpos);
        }
        return serial_number;
    } else if (errno == -ENOMSG) {
        printf("No serial number available\n");
    } else {
        perror("ERROR: HDIO_GET_IDENTITY");
    }

    throw std::exception();
}

size_t getLicenseKey() {
    auto key = getLinuxInfo();
    return std::hash<std::string>{}(key);
}
