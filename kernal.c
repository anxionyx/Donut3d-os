/* kernel.c */
typedef unsigned int uint;
typedef unsigned char uchar;

#define VGA_BUF ((uchar*)0xB8000)
#define WIDTH 80
#define HEIGHT 25

/* simple putchar to VGA text mode (attribute 0x07) */
static void putch_at(int x, int y, char c) {
    if (x<0||x>=WIDTH||y<0||y>=HEIGHT) return;
    int off = 2*(y*WIDTH + x);
    VGA_BUF[off] = c;
    VGA_BUF[off+1] = 0x07;
}

static void clear_screen() {
    for(int y=0;y<HEIGHT;y++) for(int x=0;x<WIDTH;x++) putch_at(x,y,' ');
}

/* donut adapted from Andy Sloane's ASCII donut. Renders into text grid. */
void kmain(void) {
    clear_screen();
    const float R1 = 1.0f, R2 = 2.0f;
    const float K2 = 5.0f;
    const float K1 = WIDTH * K2 * 3.0f / (8.0f * (R1 + R2));
    float A = 0, B = 0;
    while (1) {
        float zbuf[HEIGHT][WIDTH];
        char screen[HEIGHT][WIDTH];
        for(int i=0;i<HEIGHT;i++) for(int j=0;j<WIDTH;j++){ zbuf[i][j]=0; screen[i][j]=' '; }

        for(float theta=0; theta < 6.28318f; theta += 0.07f) {
            for(float phi=0; phi < 6.28318f; phi += 0.02f) {
                float cosA = cosf(A), sinA = sinf(A);
                float cosB = cosf(B), sinB = sinf(B);
                float cost = cosf(theta), sint = sinf(theta);
                float cosp = cosf(phi), sinp = sinf(phi);

                float circlex = R2 + R1 * cost;
                float x = circlex * (cosB * cosp + sinA * sinB * sinp) - R1 * sint * cosA * sinB;
                float y = circlex * (sinB * cosp - sinA * cosB * sinp) + R1 * sint * cosA * cosB;
                float z = K2 + cosA * circlex * sinp + R1 * sint * sinA;
                float ooz = 1.0f / z;

                int xp = (int)(WIDTH/2 + K1 * ooz * x);
                int yp = (int)(HEIGHT/2 - K1 * ooz * y);

                float L = cosp * cost * sinB - cosA * cost * sinp - sinA * sint + cosB * (cosA * sint - cost * sinA * sinp);
                int lumin = (int)(L * 8.0f);
                if (lumin > 0 && xp>=0 && xp<WIDTH && yp>=0 && yp<HEIGHT) {
                    if (ooz > zbuf[yp][xp]) {
                        zbuf[yp][xp] = ooz;
                        const char* shades = ".,-~:;=!*#$@";
                        screen[yp][xp] = shades[lumin > 11 ? 11 : lumin];
                    }
                }
            }
        }

        /* draw to VGA */
        for(int y=0;y<HEIGHT;y++) for(int x=0;x<WIDTH;x++) putch_at(x,y, screen[y][x] ? screen[y][x] : ' ');

        /* simple delay */
        for(volatile int i=0;i<2000000;i++);

        A += 0.07f;
        B += 0.03f;
    }
}

/* Provide cos/sin/prototypes from libm replacements (simple Taylor approx or import from compiler).
   For simplicity, declare externs and link with -lm if available. */
extern float cosf(float);
extern float sinf(float);
