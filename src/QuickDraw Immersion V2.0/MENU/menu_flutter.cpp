// menu_flutter.cpp
#include "menu_flutter.h"
#include "..\header.h"
#include <shellapi.h>

void Container::Draw() {
    if (!isVisible) return;
    
    // Desenha background
    // TODO: Implementar DrawRect com backgroundColor
    
    // Desenha children
    for (auto* child : children) {
        child->Draw();
    }
}

void Container::AddChild(Widget* child) {
    children.push_back(child);
}

void Container::Update() {
    if (!isVisible) return;
    for (auto* child : children) {
        child->Update();
    }
}

bool Container::HandleInput() {
    if (!isVisible) return false;
    for (auto* child : children) {
        if (child->HandleInput()) return true;
    }
    return false;
}

Text::Text(const std::string& text, float fontSize) 
    : text(text), fontSize(fontSize) {
    textColor = Color(255, 255, 255);
}

void Text::Draw() {
    if (!isVisible) return;
    // TODO: Implementar DrawText
}

void Text::Update() {
    // Implementação vazia por enquanto
}

bool Text::HandleInput() {
    return false;  // Texto não processa input
}

Button::Button(const std::string& label, std::function<void()> callback)
    : label(label), onPressed(callback), isHovered(false) {
    normalColor = Color(66, 66, 66);
    hoverColor = Color(33, 150, 243);
}

void Button::Draw() {
    if (!isVisible) return;
    
    Color currentColor = isHovered ? hoverColor : normalColor;
    // TODO: Implementar DrawButton
}

void Button::Update() {
    // Verifica hover state
    // TODO: Implementar verificação de mouse/controle
}

bool Button::HandleInput() {
    if (!isVisible) return false;
    if (isHovered && PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_FRONTEND_ACCEPT)) {
        if (onPressed) onPressed();
        return true;
    }
    return false;
}

FlutterMenu::FlutterMenu() {
    rootContainer = new Container();
    currentSelection = 0;
    isOpen = false;
    animationProgress = 0.0f;
    m_IsInitialized = true;
    logMessage("Menu Flutter inicializado com sucesso");
}

// Implementação completa do Update
void FlutterMenu::Update() {
    if (!m_IsInitialized) return;
    
    static DWORD lastUpdateTime = 0;
    DWORD currentTime = GetTickCount();
    
    // Limita updates a 60fps
    if (currentTime - lastUpdateTime < 16) return;
    lastUpdateTime = currentTime;
    
    ProcessInput();
    
    if (!isOpen) return;

    // Atualiza animação
    if (animationProgress < 1.0f) {
        animationProgress += 0.1f;
        if (animationProgress > 1.0f) animationProgress = 1.0f;
    }

    // Processa input de navegação
    if (m_OpenKeyPressed && !isOpen) {
        Open();
        AUDIO::PLAY_SOUND_FRONTEND("NAV_UP", "HUD_SHOP_SOUNDSET", true, 0);
    }

    if (!isOpen) return;

    if (m_BackKeyPressed) {
        Close();
        AUDIO::PLAY_SOUND_FRONTEND("NAV_DOWN", "HUD_SHOP_SOUNDSET", true, 0); 
        return;
    }

    // Navegação
    if (m_UpKeyPressed) {
        if (currentSelection > 0) {
            currentSelection--;
            AUDIO::PLAY_SOUND_FRONTEND("NAV_UP", "HUD_SHOP_SOUNDSET", true, 0);
        }
    }

    if (m_DownKeyPressed) {
        if (currentSelection < menuItems.size() - 1) {
            currentSelection++;
            AUDIO::PLAY_SOUND_FRONTEND("NAV_DOWN", "HUD_SHOP_SOUNDSET", true, 0);
        }
    }

    // Selecionar item
    if (m_EnterKeyPressed) {
        if (menuItems[currentSelection].callback) {
            menuItems[currentSelection].callback();
            AUDIO::PLAY_SOUND_FRONTEND("SELECT", "HUD_SHOP_SOUNDSET", true, 0);
        }
    }

    // Valores de opções
    if (m_LeftKeyPressed || m_RightKeyPressed) {
        AUDIO::PLAY_SOUND_FRONTEND("NAV_LEFT_RIGHT", "HUD_SHOP_SOUNDSET", true, 0);
    }
    
    Draw();
}

void FlutterMenu::Draw() {
    if (!isOpen || !m_IsInitialized) return;
    //DrawMenu();
}

// Implementação completa do ProcessInput
void FlutterMenu::ProcessInput() {
    // Verifica se está usando teclado/mouse ou gamepad
    int controlIndex = PAD::IS_USING_KEYBOARD_AND_MOUSE(0) ? INPUT_GROUP_KEYBOARD : INPUT_GROUP_GAMEPAD;

    // Adicionar verificação de debounce para evitar duplo input
    static DWORD lastInputTime = 0;
    DWORD currentTime = GetTickCount();
    if (currentTime - lastInputTime < 150) return;

    // Processa teclas do teclado
    if (controlIndex == INPUT_GROUP_KEYBOARD) {
        m_OpenKeyPressed = IsKeyJustUp(KEY_OPEN);
        m_BackKeyPressed = IsKeyJustUp(KEY_BACK);
        m_EnterKeyPressed = IsKeyJustUp(KEY_ENTER);
        m_UpKeyPressed = IsKeyJustUp(KEY_UP);
        m_DownKeyPressed = IsKeyJustUp(KEY_DOWN);
        m_LeftKeyPressed = IsKeyJustUp(KEY_LEFT); 
        m_RightKeyPressed = IsKeyJustUp(KEY_RIGHT);
    }
    // Processa botões do gamepad
    else {
        m_OpenKeyPressed = PAD::IS_DISABLED_CONTROL_PRESSED(controlIndex, INPUT_FRONTEND_RB) && 
                          PAD::IS_CONTROL_JUST_RELEASED(controlIndex, INPUT_FRONTEND_X);
        m_BackKeyPressed = PAD::IS_CONTROL_JUST_RELEASED(controlIndex, INPUT_FRONTEND_CANCEL);
        m_EnterKeyPressed = PAD::IS_CONTROL_JUST_RELEASED(controlIndex, INPUT_FRONTEND_ACCEPT);
        m_UpKeyPressed = PAD::IS_CONTROL_JUST_PRESSED(controlIndex, INPUT_FRONTEND_UP);
        m_DownKeyPressed = PAD::IS_CONTROL_JUST_PRESSED(controlIndex, INPUT_FRONTEND_DOWN);
        m_LeftKeyPressed = PAD::IS_CONTROL_JUST_PRESSED(controlIndex, INPUT_FRONTEND_LEFT);
        m_RightKeyPressed = PAD::IS_CONTROL_JUST_PRESSED(controlIndex, INPUT_FRONTEND_RIGHT);
    }

    // Atualiza o tempo do último input
    if (m_EnterKeyPressed || m_BackKeyPressed) {
        lastInputTime = currentTime;
    }

    // Desativa controles do jogo quando menu está aberto
    if (isOpen) {
        PAD::DISABLE_CONTROL_ACTION(controlIndex, INPUT_FRONTEND_PAUSE_ALTERNATE, true);
        PAD::DISABLE_CONTROL_ACTION(controlIndex, INPUT_FRONTEND_PAUSE, true);
        PAD::DISABLE_CONTROL_ACTION(controlIndex, INPUT_WHISTLE, true);
        PAD::DISABLE_CONTROL_ACTION(controlIndex, INPUT_WHISTLE_HORSEBACK, true);
        PAD::DISABLE_CONTROL_ACTION(controlIndex, INPUT_OPEN_WHEEL_MENU, true);
        PAD::DISABLE_CONTROL_ACTION(controlIndex, INPUT_OPEN_SATCHEL_MENU, true);
        PAD::DISABLE_CONTROL_ACTION(controlIndex, INPUT_OPEN_JOURNAL, true);
    }
}

void OpenWebsite() {
    // Convert to ASCII string for ShellExecuteA
    ShellExecuteA(NULL, "open", "https://www.exemplo.com", NULL, NULL, SW_SHOWNORMAL);
}

//// Implementação do menu atualizado
//void InitializeMenu() {
//    FlutterMenu menu;
//    
//    menu.AddHeader("Mod by Toddyclipsgg");
//
//#if MEMBER_PLUS
//    menu.AddItem("Discover more mods for RDR2 and GTA 6", []() {
//        OpenWebsite();
//    }, true);
//#endif
//
//    menu.AddItem("Gun Tricks", []() {
//        // Implementação dos gun tricks
//    });
//
//    menu.AddItem("Change Weather", []() {
//        // Implementação do weather system
//    });
//
//    menu.AddItem("Change Time", []() {
//        // Implementação do time system
//    });
//
//    // Adiciona suporte para animações e efeitos visuais nativos do RDR2
//    GRAPHICS::ANIMPOSTFX_PLAY("MP_Celeb_Win");
//    AUDIO::PLAY_SOUND_FRONTEND("NAV_UP", "HUD_SHOP_SOUNDSET", true, 0);
//}