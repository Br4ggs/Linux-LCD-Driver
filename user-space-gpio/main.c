/* This is a very simple test program for char devices
 * It opens the device node and writes/reads the word "yow"
 * Afterwards it releases the file again.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    //FILE *fp = NULL;

    int fp = open("/dev/chardev0", O_RDWR);
    //fp = fopen("/dev/chardev0", "r+");

    if (fp < 0)
    {
        printf("could not open file\n");
        return -1;
    }

    // if (!fp)
    // {
    //     printf("could not open file\n");
    //     return -1;
    // }

    //char buf[] = "GPYO";

    printf("setting gpio pin 4 to HIGH\n");
    // //int numBytesWritten = fwrite(buf, 3, 1, fp);
    int numBytesWritten = write(fp, "GPHI", 5);
    printf("return value: %d\n", numBytesWritten);

    sleep(3);

    printf("setting gpio pin 4 to LOW\n");
    numBytesWritten = write(fp, "GPLO", 5);
    printf("return value: %d\n", numBytesWritten);

    // //printf("read test\n");
    // //int  numBytesRead    = fread(buf, 4, 1, fp);

    // //fclose(fp);
    close(fp);

    return 0;
}
