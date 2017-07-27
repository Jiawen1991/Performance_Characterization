#ifndef __TOWDOWN_PAPI_H__
#define __TOWDOWN_PAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

void topdown_papi_init(int);
void topdown_papi_start();
void topdown_papi_end();
void topdown_papi_finish();

#ifdef __cplusplus
}
#endif

#endif /* TOWDOWN_PAPI_H */
