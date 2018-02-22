#include "chotrod.h"
int main (int argc, char* argv[])
{
    void *ctx = initCtx();
    struct char_buffer key, value, ret;
    key.buf = "ciao";
    key.count = 4;
    value.buf = "miao";
    value.count = 4;
    ret = put(ctx,key,value);
    return 0;
}
