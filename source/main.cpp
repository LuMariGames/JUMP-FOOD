#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <3ds.h>
#include <citro2d.h>
#include <limits.h>
#include <math.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include "main.h"

// SDカードからテクスチャを読み込む
const char* texturePath = "romfs:/image.t3x";
char buffer[BUFFER_SIZE];
int scene = 0, count = 0;

C2D_TextBuf g_dynamicBuf;
C2D_Text dynText;
C2D_SpriteSheet spriteSheet;

int main() {
	// 初期化
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// 描画バッファ
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	spriteSheet = C2D_SpriteSheetLoad(texturePath);

	while (aptMainLoop()) {
		hidScanInput();
		u32 kDown = hidKeysDown();
		unsigned int key = hidKeysDown();
		if (kDown & KEY_START) break;  // STARTボタンで終了

		// 描画開始
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		// 上画面に描画
		C2D_TargetClear(top, C2D_Color32(0, 0, 0, 1));
		C2D_SceneBegin(top);
		// ステージ
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(spriteSheet, 0),
			(float)(TOP_WIDTH),
			(float)(TOP_HEIGHT),
			0.5f, NULL, 1.0f, 1.0f);

		switch (scene) {
		case 0:	//タイトル画面

			C2D_DrawImageAt(C2D_SpriteSheetGetImage(spriteSheet, 1),(float)(TOP_WIDTH),(float)(40),0.5f,NULL,1.0f,1.0f);
			C2D_DrawImageAt(C2D_SpriteSheetGetImage(spriteSheet, 2),(float)(TOP_WIDTH),(float)(150),0.5f,NULL,1.0f,1.0f);
			if (key & KEY_A) scene = 1;

			break;

		case 1:	//ゲーム画面

			C2D_DrawImageAt(C2D_SpriteSheetGetImage(spriteSheet, 2),(float)(TOP_WIDTH),(float)(150),0.5f,NULL,1.0f,1.0f);
			C2D_DrawImageAt(C2D_SpriteSheetGetImage(spriteSheet, 3),(float)(100),(float)(TOP_HEIGHT),0.5f,NULL,1.0f,1.0f);
			if (key & KEY_X) ++count;

			snprintf(get_buffer(), BUFFER_SIZE, "%d", (char)count);
			draw_text(300, 0, get_buffer());
			break;
		}

		// 下画面に描画（必要に応じて描画）
		C2D_TargetClear(bot, C2D_Color32(0, 0, 0, 1));
		C2D_SceneBegin(bot);

		// 描画終了
		C3D_FrameEnd(0);
	}

	// リソースの解放
	C2D_Fini();
	C2D_SpriteSheetFree(spriteSheet);
	gfxExit();
	return 0;
}

void draw_text(float x, float y, const char *text) {

	C2D_TextBufClear(g_dynamicBuf);
	C2D_TextParse(&dynText, g_dynamicBuf, text);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_WithColor, x, y, 0.5f, 0.5f, 0.5f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
}

char *get_buffer() {

	return buffer;
}
