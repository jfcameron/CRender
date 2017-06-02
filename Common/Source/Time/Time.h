#ifndef _TIME_H
#define _TIME_H
#ifdef __cplusplus
extern "C" {
#endif

//struct Time;

typedef struct Time
{
    //**********
    // Accessors
    //**********


    //*****************
    // Member functions
    //*****************
    float (*time)  ();
    float (*deltaTime) ();

} Time;

Time* Time_New(void);
void  Time_Free(const Time*);

#ifdef __cplusplus
}
#endif
#endif
