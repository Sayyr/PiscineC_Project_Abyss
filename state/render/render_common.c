#include "render_common.h"


void draw_rect(SDL_Renderer* r,int x,int y,int w,int h,int R,int G,int B){
    SDL_Rect rr={x,y,w,h}; SDL_SetRenderDrawColor(r,R,G,B,255); SDL_RenderFillRect(r,&rr);
}
void draw_circle(SDL_Renderer* r,int cx,int cy,int radius){
    for(int w=0; w<radius*2; ++w)for(int h=0; h<radius*2; ++h){
        int dx=radius-w, dy=radius-h;
        if (dx*dx+dy*dy <= radius*radius) SDL_RenderDrawPoint(r, cx+dx, cy+dy);
    }
}
