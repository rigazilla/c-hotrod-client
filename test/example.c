#include "chotrod.h"
#include <stdio.h>
int main (int argc, char* argv[])
{
    void *ctx = initCtx();  // TODO: add basic configuration. ATM, only 127.0.0.1:11222 no tls

    // TODO: Need type to byte[] conversion (marsh/unmarsh)
    struct char_buffer key, value, ret;
    key.buf = "key";
    key.count = 4;
    value.buf = "hello";
    value.count = 6;
    ret = put(ctx,key,value);
    printf("put return value is (len,value)=(%d,%s)\n",ret.count, ret.buf);
    free(ret.buf);
    ret = get(ctx,key);
    printf("get return value is (len,value)=(%d,%s)\n",ret.count, ret.buf);
    free(ret.buf);
    return 0;
}
