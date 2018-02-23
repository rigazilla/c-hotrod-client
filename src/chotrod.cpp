#define HR_PROTO_EXPORT
#include "chotrod.h"

#include "infinispan/hotrod/ConfigurationBuilder.h"
#include "infinispan/hotrod/RemoteCacheManager.h"
#include "infinispan/hotrod/RemoteCache.h"
#include "infinispan/hotrod/Version.h"

using namespace infinispan::hotrod;

class CopyMarshaller : public Marshaller<std::vector<char> >
{
  public:
    virtual void marshall(const std::vector<char>& in, std::vector<char>& out) {
        out=in;
    }
    virtual std::vector<char>* unmarshall(const std::vector<char>& in) {
        return new std::vector<char>(in);
    }

    virtual ~CopyMarshaller() {}
    static void destroy(Marshaller<std::vector<char> > *marshaller) { delete marshaller; }
};

struct context {
            ConfigurationBuilder *builder;
            RemoteCacheManager *cacheManager;
            RemoteCache<std::vector<char>, std::vector<char> > *cache;
            };

void *initCtx()
{
    struct context *ctx = new struct context;
    ctx->builder = new ConfigurationBuilder();
    ConfigurationBuilder& builder = *ctx->builder;
    builder.addServer().host("127.0.0.1").port(11222);
    builder.protocolVersion(Configuration::PROTOCOL_VERSION_24);
    builder.balancingStrategyProducer(nullptr);
    ctx->cacheManager= new RemoteCacheManager(builder.build(), false);
    ctx->cacheManager->start();
    RemoteCacheManager& cacheManager = *ctx->cacheManager;
    CopyMarshaller *km = new CopyMarshaller();
    CopyMarshaller *vm = new CopyMarshaller();
    ctx->cache = new RemoteCache<std::vector<char>, std::vector<char> > (cacheManager.getCache<std::vector<char>, std::vector<char> >(km,
                &CopyMarshaller::destroy, vm, &CopyMarshaller::destroy));
    return (void*)ctx;
}

void destroyCtx(void *ptr) {
    struct context *ctx = (struct context *) ptr;
    if (ctx->cache) delete ctx->cache;
    if (ctx->cacheManager) {
        ctx->cacheManager->stop();
        delete ctx->cacheManager;
    }
    if (ctx->builder) {
        delete ctx->builder;
    }
    delete ctx;
}

struct char_buffer put(void *v_ctx, struct char_buffer key, struct char_buffer value) {
   struct context *ctx = (struct context*)v_ctx;
   std::vector<char> k(key.buf, key.buf+key.count);
   std::vector<char> v(value.buf, value.buf+value.count);
   std::vector<char>* retVec = ctx->cache->put(k,v);
   char_buffer ret = { nullptr, 0} ;
   if (retVec)
   {
       ret.count = retVec->size();
       ret.buf = new char[ret.count];
       std::copy(retVec->begin(), retVec->end(), ret.buf);
       delete retVec;
   }
   return ret;
}

struct char_buffer get(void *v_ctx, struct char_buffer key) {
   struct context *ctx = (struct context*)v_ctx;
   std::vector<char> k(key.buf, key.buf+key.count);
   std::vector<char>* retVec = ctx->cache->get(k);
   char_buffer ret = { nullptr, 0} ;
   if (retVec)
   {
       ret.count = retVec->size();
       ret.buf = new char[ret.count];
       std::copy(retVec->begin(), retVec->end(), ret.buf);
       delete retVec;
   }
   return ret;
}
