// menu_flutter.h
#pragma once
#include <vector>
#include <string>
#include <functional>
#include "../header.h" // Add this to include RDR2 natives
#include <windows.h>

// Add input constants
#define INPUT_GROUP_KEYBOARD 0
#define INPUT_GROUP_GAMEPAD 1
#define INPUT_FRONTEND_RB 0xA6
#define INPUT_FRONTEND_X 0xA7
#define INPUT_FRONTEND_CANCEL 0xA8 
#define INPUT_FRONTEND_ACCEPT 0xA9
#define INPUT_FRONTEND_UP 0xAA
#define INPUT_FRONTEND_DOWN 0xAB
#define INPUT_FRONTEND_LEFT 0xAC
#define INPUT_FRONTEND_RIGHT 0xAD
#define INPUT_FRONTEND_PAUSE 0xAE
#define INPUT_FRONTEND_PAUSE_ALTERNATE 0xAF
#define INPUT_WHISTLE 0xB0
#define INPUT_WHISTLE_HORSEBACK 0xB1
#define INPUT_OPEN_WHEEL_MENU 0xB2
#define INPUT_OPEN_SATCHEL_MENU 0xB3
#define INPUT_OPEN_JOURNAL 0xB4

// Estrutura para posição e tamanho
struct Rect {
    float x, y, width, height;
};

// Estrutura para cores RGBA
struct Color {
    int r, g, b, a;
    Color(int r = 255, int g = 255, int b = 255, int a = 255) 
        : r(r), g(g), b(b), a(a) {}
};

// Classe base Widget
class Widget {
protected:
    Rect bounds;
    Color color;
    bool isVisible = true;

public:
    virtual ~Widget() {} // Adicionar destrutor virtual
    virtual void Draw() = 0;
    virtual void Update() = 0;
    virtual bool HandleInput() = 0;
};

// Container Widget
class Container : public Widget {
private:
    std::vector<Widget*> children;
    float padding;
    Color backgroundColor;

public:
    Container(float padding = 10.0f) : padding(padding) {}
    void AddChild(Widget* child);
    void Draw();
    void Update();
    bool HandleInput();
};

// Text Widget
class Text : public Widget {
private:
    std::string text;
    float fontSize;
    Color textColor;

public:
    Text(const std::string& text, float fontSize = 0.5f);
    virtual void Draw() override;
    virtual void Update() override;
    virtual bool HandleInput() override;
};

// Button Widget
class Button : public Widget {
private:
    std::string label;
    std::function<void()> onPressed;
    bool isHovered;
    Color normalColor;
    Color hoverColor;

public:
    Button(const std::string& label, std::function<void()> callback);
    virtual void Draw() override;
    virtual void Update() override;
    virtual bool HandleInput() override;
};

// Menu Principal Flutter
class FlutterMenu {
private:
    // Estrutura para itens do menu
    struct MenuItem {
        std::string text;
        std::function<void()> callback;
        bool isPulsing;
        bool isHeader;
        
        MenuItem(const std::string& text = "", std::function<void()> cb = nullptr, 
                bool pulse = false, bool header = false) 
            : text(text), callback(cb), isPulsing(pulse), isHeader(header) {}
    };

    std::vector<MenuItem> menuItems;
    bool isOpen;
    float animationProgress;
    int currentSelection;
    float menuX;
    float menuY;
    float menuWidth;
    float menuHeight;
    bool m_IsInitialized;

    // Constantes de entrada
    static const int KEY_OPEN = VK_F7;
    static const int KEY_BACK = VK_BACK;
    static const int KEY_ENTER = VK_RETURN;
    static const int KEY_UP = VK_UP;
    static const int KEY_DOWN = VK_DOWN;

    void DrawBackground() {
        // Fundo do menu com animação fade
        float alpha = animationProgress * 255.0f;
        GRAPHICS::DRAW_RECT(menuX, menuY, menuWidth, 0.112f, 0, 0, 0, (int)alpha, true, true);
    }

    void DrawTitle(const std::string& title) {
        const char* literalString = MISC::VAR_STRING(10, "LITERAL_STRING", title.c_str());
        float titleScale = 0.6f;
        
        UIDEBUG::_BG_SET_TEXT_SCALE(titleScale, titleScale);
        UIDEBUG::_BG_SET_TEXT_COLOR(255, 255, 255, (int)(animationProgress * 255));
        invoke<Void>(0xBE5261939FBECB8C, true); // Centralizar texto
        UIDEBUG::_BG_DISPLAY_TEXT(literalString, menuX + 0.13f, menuY + 0.01f);

        // Linha divisória
        GRAPHICS::DRAW_RECT(menuX, menuY + 0.03f, menuWidth, 0.002f, 
                           255, 255, 255, (int)(animationProgress * 255), true, true);
    }

    void DrawItems() {
        float itemY = menuY + 0.05f;
        const float itemHeight = 0.038f;
        
        for (size_t i = 0; i < menuItems.size(); i++) {
            const MenuItem& item = menuItems[i];
            bool isSelected = (i == currentSelection);
            float alpha = animationProgress * 255.0f;

            // Background do item
            if (isSelected) {
                // Efeito pulse para item selecionado
                float pulse = (sin(MISC::GET_GAME_TIMER() * 0.005f) + 1.0f) * 0.5f;
                int pulseAlpha = 128 + (int)(pulse * 127.0f);
                GRAPHICS::DRAW_RECT(menuX, itemY, menuWidth, itemHeight,
                                  255, 0, 0, pulseAlpha, true, true);
            }
            else {
                GRAPHICS::DRAW_RECT(menuX, itemY, menuWidth, itemHeight,
                                  0, 0, 0, 190, true, true);
            }

            // Texto do item
            float textScale = item.isHeader ? 0.5f : 0.35f;
            const char* literalString = MISC::VAR_STRING(10, "LITERAL_STRING", item.text.c_str());
            
            UIDEBUG::_BG_SET_TEXT_SCALE(textScale, textScale);
            UIDEBUG::_BG_SET_TEXT_COLOR(255, 255, 255, (int)alpha);
            invoke<Void>(0xBE5261939FBECB8C, false);
            UIDEBUG::_BG_DISPLAY_TEXT(literalString, menuX + 0.007f, itemY + 0.008f);

            itemY += itemHeight;
        }
    }

public:
    FlutterMenu() : 
        isOpen(false),
        animationProgress(0.0f),
        currentSelection(0),
        menuX(0.052f),
        menuY(0.076f), 
        menuWidth(0.260f),
        menuHeight(0.0f),
        m_IsInitialized(true) {}

    void AddHeader(const std::string& text) {
        menuItems.emplace_back(text, nullptr, false, true);
        menuHeight += 0.038f;
    }

    void AddItem(const std::string& text, std::function<void()> callback, bool isPulsing = false) {
        menuItems.emplace_back(text, callback, isPulsing, false);
        menuHeight += 0.038f;
    }

    void Update() {
        if (!m_IsInitialized) return;

        // Atualiza animação
        if (isOpen && animationProgress < 1.0f) {
            animationProgress += 0.1f;
            if (animationProgress > 1.0f) animationProgress = 1.0f;
        }

        // Processa input
        if (IsKeyJustUp(KEY_OPEN)) {
            isOpen = !isOpen;
            if (isOpen) {
                AUDIO::PLAY_SOUND_FRONTEND("NAV_UP", "HUD_SHOP_SOUNDSET", true, 0);
            }
        }

        if (!isOpen) {
            animationProgress = 0.0f;
            return;
        }

        // Navegação
        if (IsKeyJustUp(KEY_UP) && currentSelection > 0) {
            currentSelection--;
            AUDIO::PLAY_SOUND_FRONTEND("NAV_UP", "HUD_SHOP_SOUNDSET", true, 0);
        }
        
        if (IsKeyJustUp(KEY_DOWN) && currentSelection < menuItems.size() - 1) {
            currentSelection++;
            AUDIO::PLAY_SOUND_FRONTEND("NAV_DOWN", "HUD_SHOP_SOUNDSET", true, 0);
        }

        if (IsKeyJustUp(KEY_ENTER)) {
            if (menuItems[currentSelection].callback) {
                menuItems[currentSelection].callback();
                AUDIO::PLAY_SOUND_FRONTEND("SELECT", "HUD_SHOP_SOUNDSET", true, 0);
            }
        }

        if (IsKeyJustUp(KEY_BACK)) {
            isOpen = false;
            AUDIO::PLAY_SOUND_FRONTEND("BACK", "HUD_SHOP_SOUNDSET", true, 0);
        }

        Draw();
    }

    void Draw() {
        if (!isOpen || !m_IsInitialized) return;

        DrawBackground();
        DrawTitle("QUICKDRAW IMMERSION V2.0");
        DrawItems();
    }

    bool IsOpen() const { return isOpen; }
};