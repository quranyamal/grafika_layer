#ifndef GAMBAR_H
#define GAMBAR_H


// variabel statik global buffer_*
// tipe data titik, warna
#include "glob_var_type.h"
#include "gambarwindow.h"
#include "skala.h"

unsigned char buffer_r[GLOBAL_LAYAR_X][GLOBAL_LAYAR_Y];
unsigned char buffer_g[GLOBAL_LAYAR_X][GLOBAL_LAYAR_Y];
unsigned char buffer_b[GLOBAL_LAYAR_X][GLOBAL_LAYAR_Y];
unsigned char buffer_a[GLOBAL_LAYAR_X][GLOBAL_LAYAR_Y];

struct Stack {
  titik data[1000000];
  int top;
};

void push(struct Stack *s, titik p);

titik pop(struct Stack *s);

//mengembalikan titik dengan nilai x dan y sesuai argumen
titik setTitik(int x, int y);

// Melakukan assign warna c pada
// posisi c pada matriks framebuffer
void bufferDrawDot(titik p, warna c);

//memasukka warna pixel pada driver
void DrawDot(titik p, warna c);

// Melakukan assign warna default ke matriks
// buffer (bukan matriks framebuffer) berupa
// segi empat yang dibentuk oleh dua titik p0 dan p1
void refreshBuffer(titik p0, titik p1);

// Memuat/assign nilai dari matriks buffer
// ke matriks framebuffer.
void loadBuffer();

//membuat bidang yang menyambungkan titik p[0] ke p[1], p[1] ke p[2], ...., p[n] ke p[0]
//dengan warna solid
void bufferDrawPlaneSolid(titik* p, warna c, warna bound_c, int sisi);

//membuat gambar dengan origin sebagai posisi
void bufferDrawPlaneSolidCitra(titik* citra, titik origin, warna fill, warna bound, int sisi);

//melakukan pewarnaan flood denganp sebagai titik api
void fill(titik p, warna c, warna bound_c);

void fillRectangle(titik p1, titik p2, warna c);

//================================================
void bufferDrawLine(titik p0, titik p1, warna c);	//mmengganti nilai pixel sehingga tergambar garis dari p0 ke p1
void bufferDrawPlane(titik* p, warna c, int sisi);			//membuat bidang yang menyambungkan titik p[0] ke p[1], p[1] ke p[2], ...., p[n] ke p[0]
//================================================
void bufferDrawCircle(titik p, int radius, warna c);	//membuat lingkaran dengan p sebagai pusat
int  dotDistance(titik p1, titik p2);		//mengembalikan jarak antara p1 dan p2
//================================================

titik getMidPoint(titik *citra, int sisi);

void drawWindow(titik windowPosition);

#endif //GAMBAR_H
