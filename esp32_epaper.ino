/*
Seeed XIAO ESP32C3
   ┌───┬────┬───┐
 2 │   │    │   │ 5V
 3 │   └────┘   │ GND
 4 │            │ 3V3
 5 │            │ 10  MOSI
 6 │            │  9  MISO
 7 │            │  8  SCK
21 │            │ 20
   └────────────┘
*/

#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
//GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(/*CS=5*/ 2, /*DC=*/ 3, /*RES=*/ 4, /*BUSY=*/ 5));
GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT> display(GxEPD2_290_C90c(/*CS=5*/ 2, /*DC=*/ 3, /*RES=*/ 4, /*BUSY=*/ 5)); // GDEM029C90 128x296, SSD1680


const char HelloWorld[] = "Hello World!";
const char HelloWeACtStudio[] = "By @tamama_design";

void setup(){
	Serial.begin(115200);
	display.init(115200,true,50,false);
	delay(2000);
	test();
	//Serial.println("helloWorld");
	//helloWorld();
	//delay(5000);
	//Serial.println("helloFullScreenPartialMode");
	//helloFullScreenPartialMode();
	//delay(1000);
	Serial.println("hibernate");
	// powerOffと同じ？とりあえず処理完了時にこれを呼ぶ
	display.hibernate();
	Serial.println("---------------------");
	Serial.println("-----example end-----");
	Serial.println("---------------------");
}

void loop() {
}

void test(){
}

void helloWorld(){
	// 画面の向き
	display.setRotation(3);
	// フォント設定
	//display.setFont(&FreeMonoBold9pt7b);
	// 文字色
	display.setTextColor(GxEPD_BLACK);
	// 原点オフセット
	int16_t tbx, tby;
	// 文字列の縦横
	uint16_t tbw, tbh;
	// 設定済みフォントの情報から
	// 原点オフセットと文字列縦横の値を取得
	display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
	//中央寄せ時のxy座標取得
	uint16_t x = ((display.width() - tbw) / 2) - tbx;
	uint16_t y = ((display.height() - tbh) / 2) - tby;
	// ディスプレイ更新対象を画面全体に設定
	display.setFullWindow();
	// firstPage()からdo文は定型文
	display.firstPage();
	unsigned long start = millis();
	do{
		// 背景塗りつぶし
		display.fillScreen(GxEPD_WHITE);
		// 描画位置（画面上下中央下揃え）
		display.setCursor(x, y-tbh);
		// 描画文字
		display.print(HelloWorld);
		// 文字色
		display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
		// 原点オフセットと文字列縦横を取得
		display.getTextBounds(HelloWeACtStudio, 0, 0, &tbx, &tby, &tbw, &tbh);
		// 中央寄せ
		x = ((display.width() - tbw) / 2) - tbx;
		// 描画位置（画面上下中央上揃え）
		display.setCursor(x, y+tbh);
		// 描画文字
		display.print(HelloWeACtStudio);
	}while (display.nextPage());
	Serial.print("display time[ms]: ");
	Serial.println(millis() - start);
}

void helloFullScreenPartialMode()
{
	const char fullscreen[] = "full screen update";
	const char fpm[] = "fast partial mode";
	const char spm[] = "slow partial mode";
	const char npm[] = "no partial mode";
	display.setPartialWindow(0, 0, display.width(), display.height());
	display.setRotation(3);
	//display.setFont(&FreeMonoBold9pt7b);
	if (display.epd2.WIDTH < 104) display.setFont(0);
	display.setTextColor(GxEPD_BLACK);
	const char* updatemode;
	if (display.epd2.hasFastPartialUpdate){
		updatemode = fpm;
	}
	else if (display.epd2.hasPartialUpdate){
		updatemode = spm;
	}
	else{
		updatemode = npm;
	}
	// do this outside of the loop
	int16_t tbx, tby; uint16_t tbw, tbh;
	// center update text
	display.getTextBounds(fullscreen, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t utx = ((display.width() - tbw) / 2) - tbx;
	uint16_t uty = ((display.height() / 4) - tbh / 2) - tby;
	// center update mode
	display.getTextBounds(updatemode, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t umx = ((display.width() - tbw) / 2) - tbx;
	uint16_t umy = ((display.height() * 3 / 4) - tbh / 2) - tby;
	// center HelloWorld
	display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
	uint16_t hwx = ((display.width() - tbw) / 2) - tbx;
	uint16_t hwy = ((display.height() - tbh) / 2) - tby;
	
	unsigned long start = millis();
	display.firstPage();
	do{
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(hwx, hwy);
		display.print(HelloWorld);
		display.setCursor(utx, uty);
		display.print(fullscreen);
		display.setCursor(umx, umy);
		display.print(updatemode);
	}
	while (display.nextPage());
	Serial.print("display time[ms]: ");
	Serial.println(millis()-start);
}

void showPartialUpdate(){
	// some useful background
	helloWorld();
	// use asymmetric values for test
	uint16_t box_x = 10;
	uint16_t box_y = 15;
	uint16_t box_w = 70;
	uint16_t box_h = 20;
	uint16_t cursor_y = box_y + box_h - 6;
	if (display.epd2.WIDTH < 104) cursor_y = box_y + 6;
	float value = 13.95;
	uint16_t incr = display.epd2.hasFastPartialUpdate ? 1 : 3;
	display.setFont(&FreeMonoBold9pt7b);
	if (display.epd2.WIDTH < 104) display.setFont(0);
	display.setTextColor(GxEPD_BLACK);
	// show where the update box is
	for (uint16_t r = 0; r < 4; r++){
		display.setRotation(r);
		display.setPartialWindow(box_x, box_y, box_w, box_h);
		display.firstPage();
		do{
			display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
			//display.fillScreen(GxEPD_BLACK);
		}while (display.nextPage());
		delay(2000);
		display.firstPage();
		do{
			display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
		}while (display.nextPage());
		delay(1000);
	}
	//return;
	// show updates in the update box
	for (uint16_t r = 0; r < 4; r++){
		display.setRotation(r);
		display.setPartialWindow(box_x, box_y, box_w, box_h);
		for (uint16_t i = 1; i <= 10; i += incr){
			display.firstPage();
			do{
				display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
				display.setCursor(box_x, cursor_y);
				display.print(value * i, 2);
			}while (display.nextPage());
			delay(500);
		}
		delay(1000);
		display.firstPage();
		do{
			display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
		}while (display.nextPage());
		delay(1000);
	}
}

