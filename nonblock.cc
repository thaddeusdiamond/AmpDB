#include "nonblock.h"                           // Include library header

int kbhit()
{
    struct timeval tv = { 0L, 0L };             // No block interval
    fd_set fds;
    FD_ZERO(&fds);                              // Get stdin fd_set
    FD_SET(0, &fds);                        
    return select(1, &fds, NULL, NULL, &tv);    // Listen on stdin non-block
}

int getch()
{
    int r;                                      // Read status
    unsigned char c;                            // Read character
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;                               // Invalid read
    } else {
        return c;                               // Valid read, return char
    }
}
