/* This is a very simple test program for char devices
 * It opens the device node and writes/reads the word "yow"
 * Afterwards it releases the file again.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int i, v = 0;

    char *str = (char *)malloc(v);

    for(i = 1; i < argc; i++) {
        v += strlen(argv[i]) + 1;
        str = (char *)realloc(str, v);

        strcat(str, argv[i]);

        if (i < argc - 1)
            strcat(str, " ");
    }

    printf("%s\n", str);

    FILE *fp = NULL;

    fp = fopen("/dev/chardev0", "r+");

    if (!fp)
    {
        printf("could not open file\n");
        return -1;
    }

    sleep(1);

    int  numBytesWritten = fwrite(str, strlen(str), 1, fp);

    fclose(fp);

    return 0;
}
