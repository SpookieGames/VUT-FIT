#include <stdio.h>

int main(int argc, char *argv[])
{
    int i = 10;
    int *ip;
    int **ipp;
    int ***ippp;
    int ****ipppp;
    ipppp=&ippp;
    ippp=&ipp;
    ipp = &ip;
    ip = &i;

    printf("Print %d", ****ipppp);
}