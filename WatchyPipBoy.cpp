#include "WatchyPipBoy.h"

#define DARKMODE true
#define STEPSGOAL 5000

const uint8_t WEATHER_ICON_WIDTH = 48;
const uint8_t WEATHER_ICON_HEIGHT = 32;

RTC_DATA_ATTR uint8_t CURRENT_PAGE = 0;
// 0 = STAT
// 1 = INV
// 2 = DATA
// 3 = MAP
const uint8_t PAGES[] = {0, 1, 2, 3};

RTC_DATA_ATTR uint8_t vaultBoyNum;

void WatchyPipBoy::drawWatchFace(){
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);

    //top menu bar
    drawTopBar();

    switch (CURRENT_PAGE) {
        case 0:
            drawStatPage();
            break;
        case 1:
            drawInvPage();
            break;
        case 2:
            drawDataPage();
            break;
        case 3:
            drawMapPage();
            break;
        default:
            drawStatPage();
            break;
    }
    //bottom text
    drawBottomText();


    // display.drawBitmap(120, 77, WIFI_CONFIGURED ? wifi : wifioff, 26, 18, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    // if(BLE_CONFIGURED){
    //     display.drawBitmap(100, 75, bluetooth, 13, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    // }
}

// All Pages
void WatchyPipBoy::drawTopBar() {
    display.setFont(&monofonto8pt7b);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(22, 14);
    display.print("STAT  INV  DATA  MAP");
}

void WatchyPipBoy::drawBottomText() {
    display.setFont(&monofonto8pt7b);
    display.setCursor(10, 195);
    display.println(CURRENT_PAGE);
    //display.println("PIP-BOY 3000 ROBCO IND.");
}

// Stat Page
void WatchyPipBoy::drawStatPage() {
    display.drawBitmap(0, 10, menu_stats, 200, 9, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    drawStatPageTime();
    drawStatPageDate();
    drawStatPageSteps();
    drawStatPageWeather();
    drawStatPageBattery();
}

void WatchyPipBoy::drawStatPageTime(){

    //draw random vault boy every 15mins
    if(currentTime.Minute % 15 == 0){
        vaultBoyNum = random(0, 3);
    }

    switch(vaultBoyNum){
        case 0:
            display.drawBitmap(70, 50, vaultboy, 57, 100, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
            break;
        case 1:
            display.drawBitmap(70, 50, vaultboypoint, 57, 100, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
            break;
        case 2:
            display.drawBitmap(60, 50, vaultboysmile, 67, 100, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
            break;
    }    

    //time border
    display.drawLine(137, 28, 200, 28, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.drawLine(137, 28, 137, 132, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.drawLine(137, 132, 157, 132, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);    
    display.drawLine(180, 132, 200, 132, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    //draw time
    display.setFont(&monofonto28pt7b);
    display.setCursor(141, 75);

    int displayHour = ((currentTime.Hour+11)%12)+1;

    if(displayHour < 10){
        display.print("0");
    }
    display.print(displayHour);

    display.setCursor(141, 125);
    if(currentTime.Minute < 10){
        display.print("0");
    }
    display.print(currentTime.Minute);

    //AMPM
    display.setFont(&monofonto8pt7b);
    display.setCursor(160, 140);
    display.print(currentTime.Hour < 11 ? "AM" : "PM");
}

void WatchyPipBoy::drawStatPageDate(){

    display.setFont(&monofonto10pt7b);
    int16_t  x1, y1;
    uint16_t w, h;
    String dayOfWeek = dayStr(currentTime.Wday);

    dayOfWeek.toUpperCase();
    display.setTextColor(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.getTextBounds(dayOfWeek, 7, 42, &x1, &y1, &w, &h);
    display.setCursor(7, 42);
    display.fillRect(x1-2, y1-2, w+4, h+4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.print(dayOfWeek);

    display.setFont(&monofonto10pt7b);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(7, 62);
    display.print(monthShortStr(currentTime.Month));
    display.print(" ");
    display.print(currentTime.Day);
    display.setCursor(7, 78);
    display.print(tmYearToCalendar(currentTime.Year));
}

void WatchyPipBoy::drawStatPageSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }

    //draw progress bar
    uint32_t stepCount = sensor.getCounter();
    uint8_t progress = (uint8_t)(stepCount * 100.0 / STEPSGOAL);
    progress = progress > 100 ? 100 : progress;
    display.drawBitmap(60, 155, gauge, 73, 10, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillRect(60+13, 155+5, (progress/2)+5, 4, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    //show step count
    display.setFont(&monofonto8pt7b);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(150, 160);
    display.print("STEPS");
    display.setCursor(150, 175);
    display.print(stepCount);
}

void WatchyPipBoy::drawStatPageBattery(){                                                                                                                                                                            
    display.drawBitmap(10, 150, battery, 37, 21, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillRect(15, 155, 27, 11, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);//clear battery segments
    int8_t batteryLevel = 0;
    float VBAT = getBatteryVoltage();
    if(VBAT > 3.90){
        batteryLevel = 3;
    }
    else if(VBAT > 3.6 && VBAT <= 3.9){
        batteryLevel = 2;
    }               
    else if(VBAT > 3.20 && VBAT <= 3.6){
        batteryLevel = 1;
    }
    else if(VBAT <= 3.20){
        batteryLevel = 0;
    }

    for(int8_t batterySegments = 0; batterySegments < batteryLevel; batterySegments++){
        display.fillRect(15 + (batterySegments * 9), 155, 7, 11, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    }
}

void WatchyPipBoy::drawStatPageWeather(){

    weatherData currentWeather = getWeatherData();

    int8_t temperature = currentWeather.temperature;
    int16_t weatherConditionCode = currentWeather.weatherConditionCode;

    display.setFont(&monofonto10pt7b);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(12, 133);
 
 
    display.print(temperature);
    display.print(currentWeather.isMetric ? "C" : "F");
    const unsigned char* weatherIcon;

    //https://openweathermap.org/weather-conditions
    if(weatherConditionCode > 801){//Cloudy
    weatherIcon = cloudy;
    }else if(weatherConditionCode == 801){//Few Clouds
    weatherIcon = cloudsun;
    }else if(weatherConditionCode == 800){//Clear
    weatherIcon = sunny;
    }else if(weatherConditionCode >=700){//Atmosphere
    weatherIcon = atmosphere;
    }else if(weatherConditionCode >=600){//Snow
    weatherIcon = snow;
    }else if(weatherConditionCode >=500){//Rain
    weatherIcon = rain;
    }else if(weatherConditionCode >=300){//Drizzle
    weatherIcon = drizzle;
    }else if(weatherConditionCode >=200){//Thunderstorm
    weatherIcon = thunderstorm;
    }else
    return;
    display.drawBitmap(5, 85, weatherIcon, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
}

// Inv Page
void WatchyPipBoy::drawInvPage() {
    display.drawBitmap(0, 10, menu_inv, 200, 9, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);

    display.setCursor(10, 60);
    display.setFont(&monofonto8pt7b);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(150, 160);
    display.print("Inv Page");
}

// Data Page
void WatchyPipBoy::drawDataPage() {
    display.setCursor(10, 60);
    display.setFont(&monofonto8pt7b);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(150, 160);
    display.print("Data Page");
}

// Map Page
void WatchyPipBoy::drawMapPage() {
    display.setCursor(10, 60);
    display.setFont(&monofonto8pt7b);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(150, 160);
    display.print("Map Page");
}

void WatchyPipBoy::changePage(int8_t pageChange) {
    const uint8_t numPages = sizeof(PAGES) / sizeof(PAGES[0]);

    CURRENT_PAGE = (CURRENT_PAGE + pageChange + numPages) % numPages;
}

void WatchyPipBoy::handleButtonPress() {
    Watchy::handleButtonPress();

    if (guiState != WATCHFACE_STATE) return;

    uint64_t wakeupBit = esp_sleep_get_ext1_wakeup_status();

    if (wakeupBit & UP_BTN_MASK) {
        changePage(1);
        showWatchFace(false);
        return;
    }

    if (wakeupBit & DOWN_BTN_MASK) {
        changePage(-1);
        showWatchFace(false);
        return;
    }
}
