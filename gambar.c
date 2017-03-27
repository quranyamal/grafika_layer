// global_* dan GLOBAL_*
#include "framebuffer_init.h"

// deklarasi fungsi
#include "gambar.h"
#include <stdio.h>
#include <math.h>

warna cRed ={255,25,25,255};
warna cGreen = {25,255,25,255};
warna cBlue = {25,25,255,255};
warna cWhite = {255,255,255,255};

titik setTitik(int x, int y){
    titik temp = {x, y};
    return temp;
}

//mengganti nilai pixel dengan posisi p pada buffer dengan warna c
void DrawDot(titik p, warna c){
    if((p.x < 1) || (p.x >= GLOBAL_LAYAR_X) || (p.y < 1) || (p.y >= GLOBAL_LAYAR_Y)){
		return ;
	}

    long int position = (p.x + global_vinfo.xoffset) * (global_vinfo.bits_per_pixel / 8) +
       (p.y + global_vinfo.yoffset) * global_finfo.line_length;

    if(global_vinfo.bits_per_pixel == 32){
        *(global_fbp + position) = c.b;
        *(global_fbp + position + 1) = c.g;
        *(global_fbp + position + 2) = c.r;
        *(global_fbp + position + 3) = c.a;
    }else{
        // assume 16 bit color
        int b = c.b;
        int g = c.g;
        int r = c.r;
        unsigned short int t = r<<11 | g << 5 | b;
        *((unsigned short int*)(global_fbp + position)) = t;
    }
}

void bufferDrawDot(titik p, warna c){
    if((p.x < 1) || (p.x >= GLOBAL_LAYAR_X) || (p.y < 1) || (p.y >= GLOBAL_LAYAR_Y)){
		return ;
	}

    buffer_b[p.x][p.y] = c.b;
    buffer_g[p.x][p.y] = c.g;
    buffer_r[p.x][p.y] = c.r;
    buffer_a[p.x][p.y] = c.a;
}

titik getMidPoint(titik *citra, int sisi) {
  titik ans;
  int i = 0, midx = 0, midy = 0;
  for (i = 0; i < sisi; i++) {
    midx += citra[i].x;
    midy += citra[i].y;
  }
  midx /= sisi; midy /= sisi;
  ans.x = midx; ans.y = midy;
  return ans;
}

//mengganti nilai seluruh pixel buffer menjadi background color untuk
void refreshBuffer(titik p0, titik p1){
    warna warna_default = {25, 25, 255, 255};

    //
    //       *1
    // *0
    //
    int i, j;
    if(p0.x < p1.x && p0.y < p1.y){
        for(i = p0.x; i < p1.x; i++)
            for(j = p0.y; j < p1.y; j++){
                buffer_r[i][j] = warna_default.r;
                buffer_g[i][j] = warna_default.g;
                buffer_b[i][j] = warna_default.b;
                buffer_a[i][j] = warna_default.a;
            }

        return;
    }


    //
    // *1
    //       *0
    //
    if(p0.x > p1.x && p0.y < p1.y){
        for(i = p1.x; i < p0.x; i++)
            for(j = p0.y; j < p1.y; j++){
                buffer_r[i][j] = warna_default.r;
                buffer_g[i][j] = warna_default.g;
                buffer_b[i][j] = warna_default.b;
                buffer_a[i][j] = warna_default.a;
            }

        return;
    }

    //
    //       *0
    // *1
    //
    if(p0.x > p1.x && p0.y > p1.y){
        for(i = p1.x; i < p0.x; i++)
            for(j = p1.y; j < p0.y; j++){
                buffer_r[i][j] = warna_default.r;
                buffer_g[i][j] = warna_default.g;
                buffer_b[i][j] = warna_default.b;
                buffer_a[i][j] = warna_default.a;
            }

        return;
    }

    //
    // *0
    //       *1
    //
    if(p0.x < p1.x && p0.y > p1.y){
        for(i = p0.x; i < p1.x; i++)
            for(j = p1.y; j < p0.y; j++){
                buffer_r[i][j] = warna_default.r;
                buffer_g[i][j] = warna_default.g;
                buffer_b[i][j] = warna_default.b;
                buffer_a[i][j] = warna_default.a;
            }

        return;
    }
}

//memasukkan nilai buffer ke driver
void loadBuffer(){
    int i, j;

    titik titik_sementara;
    warna warna_sementara;
    warna warna_kosong = {0, 0, 0, 0};
    for(i = 0; i < GLOBAL_LAYAR_X; i++)
        for(j = 0; j < GLOBAL_LAYAR_Y; j++){
            titik_sementara.x = i;
            titik_sementara.y = j;

            if(buffer_r[i][j] && buffer_g[i][j] &&
            buffer_b[i][j] && buffer_a[i][j]){
                warna_sementara.r = buffer_r[i][j];
                warna_sementara.g = buffer_g[i][j];
                warna_sementara.b = buffer_b[i][j];
                warna_sementara.a = buffer_a[i][j];
            }else{
                warna_sementara = warna_kosong;
            }

            DrawDot(titik_sementara, warna_sementara);
        }
}

void bufferDrawPlane(titik* p, warna c, int sisi){
	int i= 0;

	for (i = 0; i < sisi-1; i++) {
		bufferDrawLine(p[i], p[i+1], c);
	}

	bufferDrawLine(p[i], p[0], c);
}



void bufferDrawCircle(titik p, int radius, warna c){
    inline void bufferDrawHorizontalLine(int x1, int x2, int y, warna c)
    {
        int i; titik l;
        for (i = x1; i < x2 ; i++){
            l.x = i; l.y = y;
            bufferDrawDot(l, c);
        }
    }
    inline void plot4poin(titik p1, int x, int y, warna c)
    {
        /*
        //Lingkaran kosong
        int a = p1.x+ x;
        int b = p1.x- x;
        int d = p1.y +y;
        int e = p1.y -y;
        titik pa = {a,d};
        titik pb = {b,d};
        titik pc = {a,e};
        titik pd = {b,e};
        bufferDrawDot(pa,c);
        bufferDrawDot(pb,c);
        bufferDrawDot(pc,c);
        bufferDrawDot(pd,c); */

       // Lingkaran berisi
        bufferDrawHorizontalLine(p1.x - x, p1.x + x, p1.y + y,c);
        bufferDrawHorizontalLine(p1.x - x, p1.x + x, p1.y - y,c);
    }

    inline void plot8poin(titik p1, int x, int y, warna c)
    {
        plot4poin(p1, x, y, c);
        plot4poin(p1, y, x, c);
    }


    int error = -radius;
    int x = radius;
    int y = 0;

    while (x >= y)
    {
        plot8poin(p, x, y, c);

        error += y;
        y++;
        error += y;

        if (error >= 0)
        {
            error += -x;
            x--;
            error += -x;
        }
    }
}

int dotDistance(titik p1, titik p2){
    double hasil;
    hasil = sqrt(((p2.x-p1.x)*(p2.x-p1.x))+((p2.y-p1.y)*(p2.y-p1.y)));
    return hasil;
}

int is_color(titik p, warna c) {
  if ((p.x < 1) || (p.x >= GLOBAL_LAYAR_X) || (p.y < 1) || (p.y >= GLOBAL_LAYAR_Y)){
      return 1;
  }

	return buffer_r[p.x][p.y] == c.r && buffer_g[p.x][p.y] == c.g
	&& buffer_b[p.x][p.y] == c.b && buffer_a[p.x][p.y] == c.a;
}

void bufferDrawPlaneSolid(titik* p, warna c, warna bound_c, int sisi) {
	int i, x_mid = 0, y_mid = 0;
	titik flare_point;
	bufferDrawPlane(p, bound_c, sisi);
	for (i = 0; i < sisi; i++) {
		x_mid += p[i].x;
		y_mid += p[i].y;
	}
	flare_point.x = x_mid / sisi;
	flare_point.y = y_mid / sisi;
	fill(flare_point, c, bound_c);
}

void push(struct Stack *s, titik p) {
  s->data[s->top] = p;
  s->top += 1;
}

titik pop(struct Stack *s) {
  s->top -= 1;
  return s->data[s->top];
}

void fill(titik p, warna c, warna bound_c) {
  struct Stack s;
  s.top = 0;
  push(&s, p);

  while (s.top > 0) {
    titik p_n = pop(&s);
    if (!is_color(p_n, c) && !is_color(p_n, bound_c)) {
		  bufferDrawDot(p_n, c);
  		titik new_p = {p_n.x, p_n.y+1};
      push(&s, new_p);
  		new_p.x = p_n.x-1; new_p.y = p_n.y;
  		push(&s, new_p);
  		new_p.x = p_n.x+1; new_p.y = p_n.y;
  		push(&s, new_p);
  		new_p.x = p_n.x; new_p.y = p_n.y-1;
  		push(&s, new_p);
    }
  }
}

void bufferDrawLine(titik p0, titik p1, warna c) {
    int x0 = p0.x; int x1 = p1.x; int y0 = p0.y; int y1 = p1.y;
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;

    for(;;){
      titik temp = {x0, y0};
      bufferDrawDot(temp, c);
      if (x0==x1 && y0==y1) break;
      e2 = err;
      if (e2 >-dx) { err -= dy; x0 += sx; }
      if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void bufferDrawPlaneSolidCitra(titik *citra, titik pivot, warna c, warna bound_c, int sisi) {
  int i;
  titik* posAbs = (titik*) malloc(sisi * sizeof(titik));
  for (i = 0; i < sisi; i++) {
    posAbs[i].x = citra[i].x + pivot.x;
    posAbs[i].y = citra[i].y + pivot.y;
  }
  bufferDrawPlaneSolid(posAbs, c, bound_c, sisi);
  free(posAbs);
}

void fillRectangle(titik p1, titik p2, warna c) {
  for (int i = p1.x; i < p2.x; i++) {
    for (int j = p1.y; j < p2.y; j++) {
      titik p = {i, j};
      bufferDrawDot(p, c);
    }
  }
}
