#ifndef HEAD_H
#define HEAD_H

#ifdef __cplusplus
extern "C" {
#endif
#include <SDL2/SDL.h>

typedef struct head {
    SDL_Rect headRect;
    char dir;//L = left, R = right, U = up, D = down
    struct head *next;//parent of this node
} Head;


#ifdef __cplusplus
}
#endif

#endif /* HEAD_H */

