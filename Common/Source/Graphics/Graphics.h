#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#ifdef __cplusplus
extern "C" {
#endif

//struct Graphics;
typedef struct Graphics
{
    //**********
    // Accessors
    //**********


    //*****************
    // Member functions
    //*****************
    void (*init)  (Graphics);
    void (*draw)  (Graphics);


} Graphics;

Graphics* Graphics_New(void);
void      Graphics_Free(const Graphics*);

#ifdef __cplusplus
}
#endif

#endif