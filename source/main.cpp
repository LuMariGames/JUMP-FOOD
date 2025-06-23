#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "audio.h"

// SDカードからテクスチャを読み込む
const char* texturePath = "romfs:/image.t3x";
C2D_Font font;
char buffer[BUFFER_SIZE];
int scene = 0, count = 0;
float y = 150.0, jump = 0;

C2D_TextBuf g_dynamicBuf;
C2D_Text dynText;
C2D_SpriteSheet spriteSheet;

void Sendnotification(const char* titleText, const char* descText);

int main() {
	// 初期化
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	g_dynamicBuf = C2D_TextBufNew(4096);

	// 描画バッファ
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bot = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	spriteSheet = C2D_SpriteSheetLoad(texturePath);
	load_sound();
	font = C2D_FontLoad("romfs:/font.bcfnt");

	while (aptMainLoop()) {
		hidScanInput();
		u32 kDown = hidKeysHeld();
		unsigned int key = hidKeysHeld();
		if (kDown & KEY_START) break;  // STARTボタンで終了

		//ジャンプ部分のコード、変数「jump」が負の値なら勝手に落ちる
		y -= jump;
		jump -= 0.5;
		if (150.0 <= y) {	//おにぎりのy座標が150を越えたら止める
			y = 150.0;
			jump = 0;	//念の為初期値にする
		}

		// 描画開始
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

		// 上画面を描画
		C2D_TargetClear(top, C2D_Color32(0, 0, 0, 1));
		C2D_SceneBegin(top);
		// ステージ
		C2D_DrawImageAt(C2D_SpriteSheetGetImage(spriteSheet, 0),0,0,0.5f,NULL,1.0f,1.0f);

		//おにぎり
		C2D_DrawImageAtRotated(C2D_SpriteSheetGetImage(spriteSheet, 2),TOP_WIDTH/2,y,0.5f,0,NULL,1.0f,1.0f);

		switch (scene) {
		case 0:	//タイトル画面

			//タイトルロゴ
			C2D_DrawImageAtRotated(C2D_SpriteSheetGetImage(spriteSheet, 1),TOP_WIDTH/2,40,0.5f,0,NULL,1.0f,1.0f);
			if (key & KEY_A) scene = 1;

			break;

		case 1:	//ゲーム画面

			C2D_DrawImageAtRotated(C2D_SpriteSheetGetImage(spriteSheet, 3),80,TOP_HEIGHT/2,0.5f,0,NULL,1.0f,1.0f);
			if (149.99 <= y && key & KEY_X) {	//Xボタンが押された時に実行する部分
				play_sound(0);	//ジャンプ音
				++count;
				jump = 5;
			}

			//Xボタンを押した回数を表示
			snprintf(get_buffer(), BUFFER_SIZE, "%d", count);
			draw_text(80, (TOP_HEIGHT/2)-15, get_buffer());
			break;
		}

		// 下画面を描画（必要に応じて描画）
		C2D_TargetClear(bot, C2D_Color32(0, 0, 0, 1));
		C2D_SceneBegin(bot);

		// 描画終了
		C3D_FrameEnd(0);
	}
	if (count >= 100) Sendnotification("TEST MESSAGE", "100回ジャンプ達成おめでとうございます！");

	// リソースの解放
	C2D_TextBufDelete(g_dynamicBuf);
	C2D_FontFree(font);

	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	exit_music();
	return 0;
}

void draw_text(float x, float y, const char *text) {

	C2D_TextBufClear(g_dynamicBuf);
	C2D_TextFontParse(&dynText, font, g_dynamicBuf, text);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_WithColor | C2D_AlignCenter, x, y, 1.0f, 1.0f, 1.0f, C2D_Color32f(1.0f, 1.0f, 1.0f, 1.0f));
}

char *get_buffer() {

	return buffer;
}

void Sendnotification(const char* titleText, const wchar_t* descText) {
    // Initialize News
    newsInit();

    // Convert strings to UTF-16
    u16 title[128] = {0};
    u16 message[1024] = {0};
    
    // Convert ASCII to UTF-16
    for (size_t i = 0, j = strlen(titleText); i < j; i++) title[i] = titleText[i];
    for (size_t i = 0, j = strlen((const char*)descText); i < j; i++) message[i] = descText[i];

    // Create notification
    NEWS_AddNotification(title, strlen(titleText) + 2, message, strlen((const char*)descText) + 2, NULL, 0, false);
    newsExit();
}
