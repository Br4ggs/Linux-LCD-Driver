/* 
 * Author: Emiel van den Brink
 * 
 * Description:
 * This is a very simple test program for the chardev gpio character device
 * It opens the device node and writes commands GPHI and GPLO to the driver
 * This will set gpio pin 4 to high, wait 3 seconds, and then to low again.
 * Afterwards it releases the file again.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    int fp = open("/dev/chardev0", O_RDWR);

    if (fp < 0)
    {
        printf("could not open file\n");
        return -1;
    }

    printf("setting gpio pin 4 to HIGH\n");
    int numBytesWritten = write(fp, "GPHI", 5);
    printf("return value: %d\n", numBytesWritten);

    sleep(3);

    printf("setting gpio pin 4 to LOW\n");
    numBytesWritten = write(fp, "GPLO", 5);
    printf("return value: %d\n", numBytesWritten);

    close(fp);

    return 0;
}
