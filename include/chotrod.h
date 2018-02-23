#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

struct char_buffer
{
    char* buf;
    unsigned int count;
};

EXTERNC void* initCtx();
EXTERNC void destroyCtx(void*);
EXTERNC struct char_buffer put(void *v_ctx, struct char_buffer key, struct char_buffer value);
EXTERNC struct char_buffer get(void *v_ctx, struct char_buffer key);


#undef EXTERNC
