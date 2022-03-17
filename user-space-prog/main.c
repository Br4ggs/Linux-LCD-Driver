/* This is a very simple test program for char devices
 * It opens the device node and writes/reads the word "yow"
 * Afterwards it releases the file again.
 */

#include <stdio.h>

int main(void)
{
    FILE *fp = NULL;

    fp = fopen("/dev/chardev0", "r+");

    if (!fp)
    {
        printf("could not open file\n");
        return -1;
    }

    char buf[] = "yow";

    printf("write test\n");
    int  numBytesWritten = fwrite(buf, 4, 1, fp);
    printf("read test\n");
    int  numBytesRead    = fread(buf, 4, 1, fp);

    fclose(fp);

    return 0;
}
