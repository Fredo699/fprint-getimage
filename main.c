#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libfprint/fprint.h>

struct fp_dscv_dev** devlist;
struct fp_dscv_dev* scanner_dscv;
struct fp_driver* driver;
struct fp_dev* scanner;
struct fp_img* img;

int ret;

int main(int argc, char** argv){
    if (argc != 2){
        printf("usage: %s [filename].pgm\n", argv[0]);
        return -1;
    }

    if (getuid() != 0){
        printf("must be run as root!\nYou don't like segfaults, do you?\n");
        return -1;
    }

    if (fp_init()) return -1;
    devlist = fp_discover_devs();
    scanner_dscv = devlist[0];
    if (!scanner_dscv) return -1;
    
    driver = fp_dscv_dev_get_driver(scanner_dscv);

    scanner = fp_dev_open(scanner_dscv);
    ret = fp_dev_supports_imaging(scanner);
    if (!ret) printf("Device doesn't support imaging.\n");

    fp_dev_img_capture(scanner, 0, &img);
    if (ret) printf("there was a problem.\n");
    fp_img_save_to_file(img, argv[1]);

    printf("Device: %lu\nDriver: %lu", (long unsigned int) scanner, (long unsigned int) driver);
    return 0;
}

