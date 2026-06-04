#include "CardputerView.h"

namespace views {

M5GFX* CardputerView::Display = nullptr;

void CardputerView::initialise() {
    Display = &M5Cardputer.Display;
    Display->setRotation(1);
    Display->setTextColor(TEXT_COLOR);
    Display->fillScreen(BACKGROUND_COLOR);
    M5Cardputer.Display.setTextDatum(middle_center);

    // 直接使用 M5GFX 内置中文字体 (efontCN_24)
    Display->setFont(&fonts::efontCN_24);
}

void CardputerView::displayTopBar(const std::string& title, bool submenu, bool searchBar) {
    uint8_t marginX = 4;
    uint8_t marginY = 14;
    float offsetX; // for text align
    size_t limiter; // char limitation
    float sizeText; // pixels offset depending on text size

    clearTopBar();

    if (submenu) {
        drawSubMenuReturn(marginX+3, marginY); // for return <
        limiter = 15; // limit string size
        sizeText = 5.1; // pixels offset for each char
    } else {
        Display->setTextSize(TEXT_LARGE);
        limiter = 16;
        sizeText = 6.95;
    }
    
    if (searchBar) {
        Display->setTextColor(TEXT_COLOR);

        // Empty search query
        const std::string searchQuery = title.empty() ? translations::UI_TYPE_SEARCH : title.substr(0, limiter);
        
        // To center text
        offsetX = utils::StringUtils::getTextCenterOffset(searchQuery, Display->width(), sizeText);

        // Search icon
        drawSearchIcon(Display->width() - 20, marginY-2, 10, PRIMARY_COLOR);

        Display->setCursor(offsetX, marginY);
        Display->printf(searchQuery.c_str());
    } else {
        Display->setTextColor(TEXT_COLOR);
        
        // Format to remove 'Unknown_' and '.csv'from title, limit text
        std::string formattedTitle = utils::StringUtils::removeUnknownPrefix(title);
        formattedTitle = utils::StringUtils::removeCsvSuffix(formattedTitle).substr(0, limiter);

        // To center text
        offsetX = utils::StringUtils::getTextCenterOffset(formattedTitle, Display->width(), sizeText);

        Display->setCursor(offsetX, marginY);
        Display->printf(formattedTitle.c_str());
    }
}


void CardputerView::displaySelection(
                    const std::vector<std::string>& selectionStrings,  
                    uint16_t selectionIndex, 
                    const std::vector<std::string>& selectionDescription){

    uint8_t sizeX = Display->width() - 10; // width of each block
    uint8_t sizeY = 22; // height of each block
    uint8_t startY = 30; // height start block
    uint8_t stepY = 26; // step between each block
    uint8_t margin = DEFAULT_MARGIN;
    uint8_t startText = 41; // where text for each block starts
    uint8_t marginText; // width start block
    uint8_t rowsPerScreen = 4;
    size_t currentIndex; // up to date index
    bool selected; // track selected row
    uint16_t currentStartRow = selectionIndex / rowsPerScreen * rowsPerScreen;

    displayClearMainView();

    // for filtering with no results
    if (selectionStrings.empty()) {
        Display->drawCenterString(translations::UI_NO_RESULTS.c_str(), Display->width() / 2, Display->height() / 2);
    }

    for (size_t i = 0; i < rowsPerScreen && (currentStartRow + i) < selectionStrings.size(); ++i) {
        currentIndex = currentStartRow + i;
        selected = (currentIndex == selectionIndex);
        Display->setTextSize(TEXT_WIDE);
        
        drawRect(selected, margin, startY, sizeX, sizeY, stepY * i);

        // Margin
        marginText = DEFAULT_MARGIN * 2;

        // Main string
        std::string formattedString= utils::StringUtils::removeUnknownPrefix(selectionStrings[currentIndex]);
        formattedString = utils::StringUtils::removeCsvSuffix(formattedString);
        Display->setCursor(marginText + margin, startText + stepY * i);

        // Limiter 
        const char* limiter = "%.17s";

        // Add UNKNOWN if no string
        Display->printf(limiter, formattedString.empty() ? "UNKNOWN" : formattedString.c_str());

        // Description
        if (selectionDescription.size() != 0) {
            // Keybinds or one char description
            if (selectionDescription[currentIndex].size() == 1) {
                Display->setTextSize(TEXT_WIDE);
                Display->fillRoundRect(Display->width() - 44, startY + stepY * i - 1, 39, sizeY, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
                Display->drawRoundRect(Display->width() - 45, startY + stepY * i, 40, sizeY, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
                Display->setCursor(Display->width() - 32, startText + stepY * i);
                Display->printf(selectionDescription[currentIndex].c_str());
            // Text
            } else {
                Display->setTextSize(TEXT_TINY);
                Display->setCursor(selectionStrings[currentIndex].size() * 15, startText + stepY * i);
                Display->printf(selectionDescription[currentIndex].c_str());
            }
        }
    }
}

void CardputerView::displayScanInfo() {
    Display->fillScreen(BACKGROUND_COLOR);

    // Box frame
    Display->drawRect(1, 1, Display->width() - 1, Display->height() - 1, PRIMARY_COLOR);

    // Main title
    Display->setTextSize(TEXT_BIG);
    Display->setCursor(40, 18);
    Display->setTextColor(PRIMARY_COLOR);
    Display->printf(translations::UI_SCAN_TITLE.c_str());

    // Sub title
    Display->setTextSize(TEXT_MEDIUM);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(16, 42);
    Display->printf(translations::UI_SCAN_DESC.c_str());
    Display->setTextSize(TEXT_TINY);
    Display->setCursor(15, 59);

    // Text
    Display->printf(translations::UI_SCAN_TEXT1.c_str());
    Display->setTextColor(PRIMARY_COLOR);
    Display->setCursor(15, 72);
    Display->printf(translations::UI_SCAN_TEXT2.c_str());
    Display->setCursor(94, 72);
    Display->setTextColor(TEXT_COLOR);
    Display->printf(translations::UI_ADD_TO_FAV2.c_str());
    Display->setTextSize(TEXT_SMALL);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(18, 90);
    Display->printf(translations::UI_SCAN_TEXT4.c_str());

    // Button OK
    Display->fillRoundRect(70, 105, 100, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    Display->setTextColor(TEXT_COLOR);
    Display->setTextSize(TEXT_MEDIUM);
    Display->setCursor(80, 115);
    Display->printf(translations::UI_OK_START.c_str());
}

void CardputerView::displayScanSelection(std::string manufacturerName) {
    Display->fillRect(0, 30, Display->width(), Display->height(), BACKGROUND_COLOR);

    drawRemoteIcon(Display->width() - 55, 36, 40, 75, RECT_COLOR_DARK, RECT_COLOR_LIGHT);

    // Manufacturer
    Display->setTextSize(TEXT_WIDE);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(15, 55);
    Display->printf(manufacturerName.c_str());
}

void CardputerView::displayIrFileInfo() {
    Display->fillScreen(BACKGROUND_COLOR);

    // Box frame
    Display->drawRect(1, 1, Display->width() - 1, Display->height() - 1, PRIMARY_COLOR);

    // Main title
    Display->setTextSize(TEXT_BIG);
    Display->setCursor(50, 22);
    Display->setTextColor(PRIMARY_COLOR);
    Display->printf(translations::UI_FILE_TITLE.c_str());

    // Sub title
    Display->setTextSize(TEXT_SMALL);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(23, 46);
    Display->printf(translations::UI_FILE_DESC.c_str());

    // Text
    Display->setCursor(23, 65);
    Display->printf(translations::UI_FILE_TEXT.c_str());
    Display->setTextColor(PRIMARY_COLOR);
    Display->setCursor(45, 88);
    Display->setTextSize(TEXT_WIDE);
    Display->printf("Ultimate-Remote");

    // Button OK
    Display->fillRoundRect(70, 105, 100, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    Display->setTextColor(TEXT_COLOR);
    Display->setTextSize(TEXT_MEDIUM);
    Display->setCursor(80, 115);
    Display->printf(translations::UI_OK_START.c_str());
}

void CardputerView::displayScanRemote(std::string remoteName) {
    Display->fillRect(1, 70, 184, 19, BACKGROUND_COLOR); //clear
    Display->setTextColor(TEXT_COLOR);
    Display->setTextSize(0.65);
    Display->setCursor(15, 75);
    Display->printf(utils::StringUtils::removeCsvSuffix("Remote " + remoteName).c_str());
    Display->setTextSize(0.53);
    Display->setTextColor(PRIMARY_COLOR);
    Display->setCursor(7, 126);
    Display->printf(translations::UI_SCAN_PRESS_SPACE.c_str());
    Display->setTextColor(TEXT_COLOR);
    Display->printf(translations::UI_ADD_TO_FAV.c_str());
}

void CardputerView::displayScanCommand(std::string commandName) { 
    Display->fillRect(1, 89, 184, 20, BACKGROUND_COLOR); //clear
    Display->setTextSize(TEXT_MEDIUM);
    Display->setCursor(15, 95);
    Display->setTextColor(PRIMARY_COLOR);
    Display->printf("%.16s", utils::StringUtils::removeCsvSuffix(commandName).c_str());
}

void CardputerView::displayScanOver() { 
    Display->fillScreen(BACKGROUND_COLOR);

    // Box frame
    Display->drawRect(1, 1, Display->width() - 1, Display->height() - 1, PRIMARY_COLOR);

    // Main title
    Display->setTextSize(TEXT_BIG);
    Display->setTextColor(PRIMARY_COLOR);
    Display->setCursor(16, 18);
    Display->printf(translations::UI_SCAN_COMPLETE.c_str());

    // Sub title
    Display->setTextSize(TEXT_MEDIUM);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(20, 42);
    Display->printf(translations::UI_SCAN_ALL_TRIED.c_str());
    Display->setCursor(40, 59);
    Display->printf(translations::UI_SCAN_TEXT3.c_str());

    // Text
    Display->setCursor(14, 76);
    Display->printf(translations::UI_SCAN_NOT_FOUND.c_str());
    Display->setCursor(11, 92);
    Display->setTextColor(PRIMARY_COLOR);
    Display->printf(translations::UI_SCAN_TRY_FAV.c_str());

    // Button OK
    Display->fillRoundRect(75, 105, 90, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(85, 115);
    Display->printf(translations::UI_OK_QUIT.c_str());
}

void CardputerView::displayStringPrompt(std::string stringDescription, std::string stringInput) {

    // Clear
    displayClearMainView(5);

    // Box frame
    Display->drawRoundRect(10, 35, Display->width() - 20, 90, DEFAULT_ROUND_RECT, PRIMARY_COLOR);

    // Description
    Display->setTextSize(TEXT_MEDIUM);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(40, 47);
    Display->printf(stringDescription.c_str());

    // input
    Display->setTextSize(TEXT_MEDIUM_LARGE);
    Display->drawRoundRect(42, 62, 155, 25, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
    Display->setCursor(51, 73);
    Display->printf(stringInput.c_str());

    // < button
    Display->drawRoundRect(53, 95, 40, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    Display->setCursor(70, 103);
    Display->printf(translations::UI_BACK.c_str());

    // Button save
    if (stringInput.empty()) {
        Display->drawRoundRect(105, 95, 80, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    } else {
        Display->fillRoundRect(105, 95, 80, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    }
    Display->setTextSize(TEXT_MEDIUM);
    Display->setCursor(125, 105);
    Display->printf(translations::UI_SAVE.c_str());
}

void CardputerView::displayConfirmationPrompt(std::string stringDescription) {
    // Clear
    displayClearMainView(5);

    // Box frame
    Display->drawRoundRect(10, 35, Display->width() - 20, 90, DEFAULT_ROUND_RECT, PRIMARY_COLOR);

    // Description
    Display->setTextSize(TEXT_MEDIUM);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(57, 62);
    Display->printf(stringDescription.c_str());
    Display->setTextSize(TEXT_MEDIUM);

    // < button
    Display->drawRoundRect(65, 85, 40, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    Display->setCursor(82, 94);
    Display->printf(translations::UI_BACK.c_str());

    // ok button
    Display->fillRoundRect(128, 85, 40, 20, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
    Display->setCursor(138, 95);
    Display->printf(translations::UI_OK.c_str());
}

void CardputerView::displayLoading() {
    // Clear
    displayClearMainView(5);

    // Box frame
    Display->drawRoundRect(10, 35, Display->width() - 20, 90, DEFAULT_ROUND_RECT, PRIMARY_COLOR);

    // Description
    Display->setTextSize(TEXT_WIDE);
    Display->setTextColor(TEXT_COLOR);
    Display->setCursor(77, 80);
    Display->printf(translations::UI_LOADING.c_str());
    Display->setTextSize(TEXT_MEDIUM);
}


void CardputerView::drawSearchIcon(int x, int y, int size, uint16_t color) {
    int radius = size / 2;
    int handleLength = size / 2;

    // Dessiner cercle
    Display->drawCircle(x, y, radius, color);
    // Dessiner poignée
    Display->drawLine(x + radius, y + radius, x + radius + handleLength, y + radius + handleLength, color);
}

void CardputerView::drawRemoteIcon(int startX, int startY, int width, int height, uint16_t iconColor, uint16_t buttonColor) {
    // Dessiner la remote
    Display->fillRoundRect(startX, startY, width, height, DEFAULT_ROUND_RECT*2, iconColor);
    Display->drawRoundRect(startX, startY, width, height, DEFAULT_ROUND_RECT*2, PRIMARY_COLOR);

    // Calculer les dimensions 
    int buttonWidth = width / 6;
    int buttonHeight = height / 10;
    int buttonSpacing = (width - 2 * buttonWidth) / 5;
    int verticalSpacing = (height - 3 * buttonHeight) / 6;
    
    // Dessiner les boutons
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
            int buttonX = startX + buttonSpacing + j * (buttonWidth + buttonSpacing);
            int buttonY = startY + verticalSpacing + i * (buttonHeight + verticalSpacing);
            
            // No buttons for those indexes
            if (i == 0 && j == 0) continue;
            if (i == 0 && j == 1) continue;

            // color primary for centered button 
            if (i == 2 && j == 1) {
                Display->fillRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
            } else {
                Display->fillRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, DEFAULT_ROUND_RECT, buttonColor);
            }
        }
    }
}

void CardputerView::drawRect(bool selected, uint8_t margin, uint16_t startY, uint16_t sizeX, uint16_t sizeY, uint16_t stepY) {
        // Draw rect
        if (selected) {
            Display->fillRoundRect(margin, startY + stepY, sizeX, sizeY, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
            Display->setTextColor(TEXT_COLOR);
        } else {
            Display->fillRoundRect(margin, startY + stepY , sizeX, sizeY, DEFAULT_ROUND_RECT, RECT_COLOR_DARK);
            Display->drawRoundRect(margin, startY + stepY, sizeX, sizeY, DEFAULT_ROUND_RECT, PRIMARY_COLOR);
            Display->setTextColor(TEXT_COLOR);
        }
}

void CardputerView::drawSubMenuReturn(uint8_t x, uint8_t y) {
    Display->setTextSize(TEXT_WIDE);
    Display->setTextColor(PRIMARY_COLOR);
    Display->setCursor(x, y);
    Display->printf(translations::UI_BACK.c_str());
}

void CardputerView::displayClearMainView(uint8_t offsetY) {
    Display->fillRect(0, TOP_BAR_HEIGHT-offsetY, Display->width(), Display->height(), BACKGROUND_COLOR);
}

void CardputerView::clearTopBar() {
    Display->fillRect(0, 0, Display->width(), TOP_BAR_HEIGHT, BACKGROUND_COLOR);
}

}
