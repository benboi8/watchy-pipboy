#ifndef WATCHY_PIP_BOY_H
#define WATCHY_PIP_BOY_H

#include <Watchy.h>
#include "monofonto28pt7b.h"
#include "monofonto10pt7b.h"
#include "monofonto8pt7b.h"
#include "monofonto6pt7b.h"
#include "img.h"
#include "icons.h"

class WatchyPipBoy : public Watchy{
    using Watchy::Watchy;
    public:
        void drawTopBar();
        void drawBottomText();
        void drawEars(char * string, uint8_t x_offset);

        void drawStatPage();
        void drawWatchFace();
        void drawStatPageTime();
        void drawStatPageDate();
        void drawStatPageSteps();
        void drawStatPageBattery();
        void drawStatPageWeather();

        void drawInvPage();
        void drawDataPage();
        void drawMapPage();

        void changePage(int8_t pageChange);
        void handleButtonPress() override;
};

#endif
