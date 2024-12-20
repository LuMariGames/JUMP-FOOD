#pragma once

#define TOP_WIDTH  400
#define TOP_HEIGHT 240
#define BOTTOM_WIDTH  320
#define BOTTOM_HEIGHT 240
#define BUFFER_SIZE 160

void draw_text(float x, float y, const char *text);
char *get_buffer();
