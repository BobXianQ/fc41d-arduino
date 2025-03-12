#include <Arduino.h>
#include "StreamString.h"

size_t StreamString::write(const uint8_t *data, size_t size) {
    if(size && data) {
        const unsigned int newlen = length() + size;
        if(reserve(newlen + 1)) {
            memcpy((void *) (wbuffer() + len()), (const void *) data, size);
            setLen(newlen);
            *(wbuffer() + newlen) = 0x00; // add null for string end
            return size;
        }
    }
    return 0;
}

size_t StreamString::write(uint8_t data) {
    return concat((char) data);
}

int StreamString::available() {
    return length();
}

int StreamString::read() {
    if(length()) {
        char c = charAt(0);
        remove(0, 1);
        return c;

    }
    return -1;
}

int StreamString::peek() {
    if(length()) {
        char c = charAt(0);
        return c;
    }
    return -1;
}

void StreamString::flush() {
}

