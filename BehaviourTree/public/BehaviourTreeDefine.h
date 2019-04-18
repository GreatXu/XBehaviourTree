#ifndef BehaviourDefine_h
#define BehaviourDefine_h

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

typedef void(_stdcall* DebugCallBack) (const char* str);

#endif
