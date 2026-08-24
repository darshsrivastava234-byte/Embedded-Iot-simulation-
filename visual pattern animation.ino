#define CLK 13
#define DIN 11
#define CS 10

#define X_SEGMENTS 4
#define Y_SEGMENTS 4
#define NUM_SEGMENTS (X_SEGMENTS * Y_SEGMENTS)

#define WIDTH  (X_SEGMENTS * 8)
#define HEIGHT (Y_SEGMENTS * 8)

byte fb[8 * NUM_SEGMENTS];


// ============================================================
// SEND DATA TO ALL MAX7219
// ============================================================

void shiftAll(byte send_to_address, byte send_this_data)
{
  digitalWrite(CS, LOW);

  for (int i = 0; i < NUM_SEGMENTS; i++)
  {
    shiftOut(DIN, CLK, MSBFIRST, send_to_address);
    shiftOut(DIN, CLK, MSBFIRST, send_this_data);
  }

  digitalWrite(CS, HIGH);
}


// ============================================================
// SETUP
// ============================================================

void setup()
{
  Serial.begin(115200);

  pinMode(CLK, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(CS, OUTPUT);

  // MAX7219 configuration
  shiftAll(0x0F, 0x00);   // Display test OFF
  shiftAll(0x0B, 0x07);   // Scan limit 0-7
  shiftAll(0x0C, 0x01);   // Normal operation
  shiftAll(0x0A, 0x0F);   // Maximum brightness
  shiftAll(0x09, 0x00);   // No decode

  clear();
  show();

  randomSeed(analogRead(A0));
}


// ============================================================
// MAIN LOOP
// ============================================================

void loop()
{
  // ----------------------------------------------------------
  // 1. HELLO
  // ----------------------------------------------------------

  showText("HELLO", 150);

  delay(400);


  // ----------------------------------------------------------
  // 2. RADIAL WAVE
  // ----------------------------------------------------------

  radialWave();

  delay(300);


  // ----------------------------------------------------------
  // 3. ROTATING DIAMOND
  // ----------------------------------------------------------

  rotatingDiamond();

  delay(300);


  // ----------------------------------------------------------
  // 4. DIAGONAL DIGITAL WAVE
  // ----------------------------------------------------------

  diagonalWave();

  delay(300);


  // ----------------------------------------------------------
  // 5. STAR EXPLOSION
  // ----------------------------------------------------------

  starExplosion();

  delay(300);


  // ----------------------------------------------------------
  // 6. SPIRAL
  // ----------------------------------------------------------

  spiralAnimation();

  delay(300);


  // ----------------------------------------------------------
  // 7. FULL SCREEN RAIN
  // ----------------------------------------------------------

  rainAnimation();

  delay(300);


  // ----------------------------------------------------------
  // 8. EXPANDING CIRCLE
  // ----------------------------------------------------------

  expandingCircle();

  delay(300);


  // ----------------------------------------------------------
  // 9. BYE
  // ----------------------------------------------------------

  showText("BYE", 180);

  delay(1200);

  clear();
  show();

  delay(500);
}


// ============================================================
// FRAMEBUFFER PIXEL FUNCTIONS
// ============================================================

void set_pixel(uint8_t x, uint8_t y, uint8_t mode)
{
  if (x >= WIDTH || y >= HEIGHT)
    return;

  byte *addr = &fb[x / 8 + y * X_SEGMENTS];

  byte mask = 128 >> (x % 8);

  switch (mode)
  {
    case 0:
      *addr &= ~mask;
      break;

    case 1:
      *addr |= mask;
      break;

    case 2:
      *addr ^= mask;
      break;
  }
}


void safe_pixel(uint8_t x, uint8_t y, uint8_t mode)
{
  if (x >= WIDTH || y >= HEIGHT)
    return;

  set_pixel(x, y, mode);
}


void pixel(int x, int y)
{
  if (x >= 0 && x < WIDTH &&
      y >= 0 && y < HEIGHT)
  {
    set_pixel(x, y, 1);
  }
}


// ============================================================
// CLEAR DISPLAY
// ============================================================

void clear()
{
  for (int i = 0; i < 8 * NUM_SEGMENTS; i++)
  {
    fb[i] = 0;
  }
}


// ============================================================
// DISPLAY FRAMEBUFFER
// ============================================================

void show()
{
  for (byte row = 0; row < 8; row++)
  {
    digitalWrite(CS, LOW);

    for (int segment = NUM_SEGMENTS - 1; segment >= 0; segment--)
    {
      byte x = segment % X_SEGMENTS;
      byte y = (segment / X_SEGMENTS) * 8;

      byte addr = (row + y) * X_SEGMENTS;

      if ((segment / X_SEGMENTS) & 1)
      {
        shiftOut(DIN, CLK, MSBFIRST, 8 - row);
        shiftOut(DIN, CLK, LSBFIRST,
                 fb[addr + x]);
      }
      else
      {
        shiftOut(DIN, CLK, MSBFIRST, 1 + row);
        shiftOut(DIN, CLK, MSBFIRST,
                 fb[addr - x + X_SEGMENTS - 1]);
      }
    }

    digitalWrite(CS, HIGH);
  }
}


// ============================================================
// DRAW LINE - BRESENHAM
// ============================================================

void drawLine(int x0, int y0, int x1, int y1)
{
  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;

  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;

  int err = dx + dy;

  while (true)
  {
    pixel(x0, y0);

    if (x0 == x1 && y0 == y1)
      break;

    int e2 = 2 * err;

    if (e2 >= dy)
    {
      err += dy;
      x0 += sx;
    }

    if (e2 <= dx)
    {
      err += dx;
      y0 += sy;
    }
  }
}


// ============================================================
// FONT - 5x7
// ============================================================

const byte font5x7[][5] =
{
  // A
  {0x7E, 0x09, 0x09, 0x7E, 0x00},

  // B
  {0x7F, 0x49, 0x49, 0x36, 0x00},

  // C
  {0x3E, 0x41, 0x41, 0x22, 0x00},

  // D
  {0x7F, 0x41, 0x41, 0x3E, 0x00},

  // E
  {0x7F, 0x49, 0x49, 0x41, 0x00},

  // H
  {0x7F, 0x08, 0x08, 0x7F, 0x00},

  // L
  {0x7F, 0x40, 0x40, 0x40, 0x00},

  // O
  {0x3E, 0x41, 0x41, 0x3E, 0x00},

  // Y
  {0x07, 0x08, 0x70, 0x08, 0x07}
};


// ============================================================
// DRAW CHARACTER
// ============================================================

void drawChar(char c, int x, int y)
{
  int index = -1;

  if (c == 'A') index = 0;
  if (c == 'B') index = 1;
  if (c == 'C') index = 2;
  if (c == 'D') index = 3;
  if (c == 'E') index = 4;
  if (c == 'H') index = 5;
  if (c == 'L') index = 6;
  if (c == 'O') index = 7;
  if (c == 'Y') index = 8;

  if (index < 0)
    return;

  for (int col = 0; col < 5; col++)
  {
    byte data = font5x7[index][col];

    for (int row = 0; row < 7; row++)
    {
      if (data & (1 << row))
      {
        pixel(x + col, y + row);
      }
    }
  }
}


// ============================================================
// DISPLAY TEXT
// ============================================================

void showText(const char *text, int holdTime)
{
  clear();

  int length = strlen(text);
  int textWidth = length * 6;

  int startX = (WIDTH - textWidth) / 2;
  int startY = (HEIGHT - 7) / 2;

  for (int i = 0; i < length; i++)
  {
    drawChar(text[i],
             startX + i * 6,
             startY);
  }

  show();

  delay(holdTime);

  // Move text toward the right
  for (int i = 0; i < 8; i++)
  {
    clear();

    for (int j = 0; j < length; j++)
    {
      drawChar(text[j],
               startX + j * 6 + i,
               startY);
    }

    show();
    delay(70);
  }

  clear();
  show();
}


// ============================================================
// 1. RADIAL WAVE
// FULL SCREEN RIPPLE EFFECT
// ============================================================

void radialWave()
{
  int cx = WIDTH / 2;
  int cy = HEIGHT / 2;

  for (int r = 1; r <= 30; r += 2)
  {
    clear();

    // Draw multiple expanding rings
    for (int ring = r; ring > 0; ring -= 6)
    {
      drawCircle(cx, cy, ring);
    }

    show();

    delay(90);
  }

  // Reverse wave
  for (int r = 30; r >= 1; r -= 2)
  {
    clear();

    for (int ring = r; ring > 0; ring -= 6)
    {
      drawCircle(cx, cy, ring);
    }

    show();

    delay(60);
  }

  clear();
  show();
}


// ============================================================
// 2. ROTATING DIAMOND
// FULL SCREEN GEOMETRIC PATTERN
// ============================================================

void rotatingDiamond()
{
  int cx = WIDTH / 2;
  int cy = HEIGHT / 2;

  for (int frame = 0; frame < 32; frame++)
  {
    clear();

    int s = frame;

    // Main diamond
    drawLine(cx, cy - s,
             cx + s, cy);

    drawLine(cx + s, cy,
             cx, cy + s);

    drawLine(cx, cy + s,
             cx - s, cy);

    drawLine(cx - s, cy,
             cx, cy - s);


    // Second diamond
    int s2 = s - 8;

    if (s2 > 0)
    {
      drawLine(cx, cy - s2,
               cx + s2, cy);

      drawLine(cx + s2, cy,
               cx, cy + s2);

      drawLine(cx, cy + s2,
               cx - s2, cy);

      drawLine(cx - s2, cy,
               cx, cy - s2);
    }


    // Outer frame connections
    if (s > 15)
    {
      drawLine(0, 0, cx, cy - s);
      drawLine(WIDTH - 1, 0, cx + s, cy);
      drawLine(WIDTH - 1, HEIGHT - 1,
               cx, cy + s);
      drawLine(0, HEIGHT - 1,
               cx - s, cy);
    }

    show();

    delay(70);
  }

  delay(200);

  clear();
  show();
}


// ============================================================
// 3. DIAGONAL DIGITAL WAVE
// FILLS ENTIRE 32x32 DISPLAY
// ============================================================

void diagonalWave()
{
  for (int offset = -32; offset < 64; offset += 2)
  {
    clear();

    for (int y = 0; y < HEIGHT; y++)
    {
      for (int x = 0; x < WIDTH; x++)
      {
        int diagonal = x + y;

        if ((diagonal - offset) >= 0 &&
            (diagonal - offset) < 5)
        {
          pixel(x, y);
        }
      }
    }

    // Additional crossing wave
    for (int y = 0; y < HEIGHT; y++)
    {
      for (int x = 0; x < WIDTH; x++)
      {
        int diagonal = x - y;

        if ((diagonal - (offset - 32)) >= 0 &&
            (diagonal - (offset - 32)) < 3)
        {
          pixel(x, y);
        }
      }
    }

    show();

    delay(45);
  }

  clear();
  show();
}


// ============================================================
// 4. STAR EXPLOSION
// CENTER -> FULL SCREEN -> CENTER
// ============================================================

void starExplosion()
{
  int cx = WIDTH / 2;
  int cy = HEIGHT / 2;

  // EXPAND
  for (int r = 1; r <= 32; r += 2)
  {
    clear();

    // Main rays
    drawLine(cx, cy, cx + r, cy);
    drawLine(cx, cy, cx - r, cy);
    drawLine(cx, cy, cx, cy + r);
    drawLine(cx, cy, cx, cy - r);

    // Diagonal rays
    drawLine(cx, cy, cx + r, cy + r);
    drawLine(cx, cy, cx - r, cy - r);
    drawLine(cx, cy, cx + r, cy - r);
    drawLine(cx, cy, cx - r, cy + r);

    // Extra rays
    drawLine(cx, cy, cx + r, cy + r / 2);
    drawLine(cx, cy, cx - r, cy - r / 2);
    drawLine(cx, cy, cx + r / 2, cy - r);
    drawLine(cx, cy, cx - r / 2, cy + r);

    show();

    delay(70);
  }

  // CONTRACT
  for (int r = 32; r >= 1; r -= 2)
  {
    clear();

    drawLine(cx, cy, cx + r, cy);
    drawLine(cx, cy, cx - r, cy);
    drawLine(cx, cy, cx, cy + r);
    drawLine(cx, cy, cx, cy - r);

    drawLine(cx, cy, cx + r, cy + r);
    drawLine(cx, cy, cx - r, cy - r);
    drawLine(cx, cy, cx + r, cy - r);
    drawLine(cx, cy, cx - r, cy + r);

    drawLine(cx, cy, cx + r, cy + r / 2);
    drawLine(cx, cy, cx - r, cy - r / 2);
    drawLine(cx, cy, cx + r / 2, cy - r);
    drawLine(cx, cy, cx - r / 2, cy + r);

    show();

    delay(45);
  }

  clear();
  show();
}


// ============================================================
// 5. SPIRAL ANIMATION
// FULL SCREEN SPIRAL
// ============================================================

void spiralAnimation()
{
  int left = 0;
  int right = WIDTH - 1;
  int top = 0;
  int bottom = HEIGHT - 1;

  for (int frame = 0; frame < 16; frame++)
  {
    clear();

    int l = left + frame;
    int r = right - frame;
    int t = top + frame;
    int b = bottom - frame;

    if (l >= r || t >= b)
      break;

    // Top
    drawLine(l, t, r, t);

    // Right
    drawLine(r, t, r, b);

    // Bottom
    drawLine(r, b, l, b);

    // Left
    drawLine(l, b, l, t + 2);

    // Inner diagonal connections
    if (frame % 2 == 0)
    {
      drawLine(l, t, r, b);
      drawLine(r, t, l, b);
    }

    show();

    delay(100);
  }

  delay(300);

  // Reverse-style spiral
  for (int frame = 15; frame >= 0; frame--)
  {
    clear();

    int l = left + frame;
    int r = right - frame;
    int t = top + frame;
    int b = bottom - frame;

    if (l >= r || t >= b)
      continue;

    drawLine(l, t, r, t);
    drawLine(r, t, r, b);
    drawLine(r, b, l, b);
    drawLine(l, b, l, t);

    if (frame % 2 == 0)
    {
      drawLine(l, t, r, b);
      drawLine(r, t, l, b);
    }

    show();

    delay(60);
  }

  clear();
  show();
}


// ============================================================
// 6. FULL SCREEN RAIN
// ============================================================

#define MAX_DROPS 55

struct Drop
{
  byte x;
  byte y;
  byte speed;
  byte length;
};

Drop drops[MAX_DROPS];


void initializeRain()
{
  for (int i = 0; i < MAX_DROPS; i++)
  {
    drops[i].x = random(WIDTH);
    drops[i].y = random(HEIGHT);
    drops[i].speed = random(1, 4);
    drops[i].length = random(2, 6);
  }
}


void rainAnimation()
{
  initializeRain();

  for (int frame = 0; frame < 150; frame++)
  {
    clear();

    for (int i = 0; i < MAX_DROPS; i++)
    {
      // Draw long rain trails
      for (int j = 0; j < drops[i].length; j++)
      {
        int yy = drops[i].y - j;

        if (yy >= 0 && yy < HEIGHT)
        {
          pixel(drops[i].x, yy);
        }
      }

      // Move
      drops[i].y += drops[i].speed;

      // Reset
      if (drops[i].y >= HEIGHT + drops[i].length)
      {
        drops[i].y = random(-8, 0);
        drops[i].x = random(WIDTH);
        drops[i].speed = random(1, 4);
        drops[i].length = random(2, 6);
      }
    }

    show();

    delay(40);
  }

  clear();
  show();
}


// ============================================================
// 7. DRAW CIRCLE
// ============================================================

void drawCircle(int cx, int cy, int r)
{
  int x = r;
  int y = 0;

  int err = 0;

  while (x >= y)
  {
    pixel(cx + x, cy + y);
    pixel(cx + y, cy + x);
    pixel(cx - y, cy + x);
    pixel(cx - x, cy + y);

    pixel(cx - x, cy - y);
    pixel(cx - y, cy - x);
    pixel(cx + y, cy - x);
    pixel(cx + x, cy - y);

    y++;

    if (err <= 0)
    {
      err += 2 * y + 1;
    }
    else
    {
      x--;
      err += 2 * (y - x) + 1;
    }
  }
}


// ============================================================
// 8. EXPANDING CIRCLE
// FULL SCREEN CONCENTRIC RINGS
// ============================================================

void expandingCircle()
{
  int cx = WIDTH / 2;
  int cy = HEIGHT / 2;

  // EXPAND
  for (int r = 1; r <= 23; r++)
  {
    clear();

    // Multiple rings
    drawCircle(cx, cy, r);

    if (r > 6)
      drawCircle(cx, cy, r - 6);

    if (r > 12)
      drawCircle(cx, cy, r - 12);

    if (r > 18)
      drawCircle(cx, cy, r - 18);

    show();

    delay(80);
  }

  delay(300);

  // CONTRACT
  for (int r = 23; r >= 1; r--)
  {
    clear();

    drawCircle(cx, cy, r);

    if (r > 6)
      drawCircle(cx, cy, r - 6);

    if (r > 12)
      drawCircle(cx, cy, r - 12);

    if (r > 18)
      drawCircle(cx, cy, r - 18);

    show();

    delay(55);
  }

  clear();
  show();
}