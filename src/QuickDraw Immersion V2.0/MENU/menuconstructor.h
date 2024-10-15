#pragma once

// Declara��o de vari�veis globais para controlar o estado do menu
int submenu = 0; // Armazena o submenu atual
int submenuLevel; // N�vel atual do submenu (profundidade)
int lastSubmenu[20]; // Armazena o �ltimo submenu visitado em cada n�vel
int lastOption[20]; // Armazena a �ltima op��o selecionada em cada n�vel de submenu
int lastSubmenuMinOptions[20]; // Armazena a contagem m�nima de op��es no �ltimo submenu
int lastSubmenuMaxOptions[20]; // Armazena a contagem m�xima de op��es no �ltimo submenu
int currentOption; // Armazena a op��o atualmente selecionada
int optionCount; // Contagem total de op��es no submenu atual
int maxOptions = 8; // N�mero m�ximo de op��es que podem ser exibidas no menu
int currentMenuMaxOptions = maxOptions; // N�mero m�ximo de op��es no menu atual
int currentMenuMinOptions = 1; // N�mero m�nimo de op��es no menu atual
float menuX = 0.052f; // Posi��o horizontal do menu na tela
bool menuHeader = false; // Flag para indicar se h� um cabe�alho no menu

// Vari�veis de controle para inputs adicionais
int m_ControlIndex = INPUT_GROUP_KEYBOARD;
bool m_OpenKeyPressed = false;
bool m_BackKeyPressed = false;
bool m_EnterKeyPressed = false;
bool m_UpKeyPressed = false;
bool m_DownKeyPressed = false;
bool m_LeftKeyPressed = false;
bool m_RightKeyPressed = false;
bool m_LMBPressed = false;

// Fun��o nula, que n�o faz nada. Usada como um valor padr�o para fun��es.
void NULLVOID() {}

// Fun��o que fecha o menu e reseta as vari�veis relacionadas ao submenu
void CloseMenu()
{
    submenu = Closed; // Define o submenu como fechado
    submenuLevel = 0; // Reseta o n�vel do submenu
    currentOption = 1; // Reseta a op��o selecionada para a primeira
}

// Fun��o para mudar o submenu atual para um novo submenu
void changeSubmenu(int newSubmenu)
{
    // Armazena o submenu e a op��o atuais antes de mudar
    lastSubmenu[submenuLevel] = submenu;
    lastOption[submenuLevel] = currentOption;
    lastSubmenuMinOptions[submenuLevel] = currentMenuMinOptions;
    lastSubmenuMaxOptions[submenuLevel] = currentMenuMaxOptions;

    // Reseta as vari�veis do novo submenu
    currentOption = 1;
    currentMenuMinOptions = 1;
    currentMenuMaxOptions = maxOptions;
    submenu = newSubmenu; // Atualiza para o novo submenu
    submenuLevel++; // Aumenta o n�vel do submenu
    m_EnterKeyPressed = false; // Reseta a flag de press�o da op��o
}

// Fun��o para adicionar um t�tulo ao menu
void addTitle(const char* title) {
    optionCount = 0; // Reseta a contagem de op��es para o t�tulo
    draw_TextWithFont(title, menuX + 0.13f, 0.076f, 255, 255, 255, 255, 16, true, 0.9f, 0.9f);
    // draw_Text(title, menuX + 0.13f, 0.076f, 255, 255, 255, 255, true, 0.5f, 0.5f); // Desenha o t�tulo
    drawRect(menuX, 0.050f, 0.260f, 0.112f, 0, 0, 0, 255); // Desenha o fundo do t�tulo
    // Desenha sprites espec�ficas para o cabe�alho do menu
    // DrawSprite("honor_display", "honor_bad", menuX, 0.058f, 0.260f, 0.074f, 0, 255, 255, 255, 255);
    DrawSprite("generic_textures", "hud_menu_4a", menuX, 0.131f + 0.027f, 0.260f, 0.002f, 0, 255, 255, 255, 255);
}

// Fun��o para adicionar um cabe�alho ao menu
void addHeader(const char* header)
{
    menuHeader = true; // Marca que o cabe�alho est� ativo
    draw_Text(header, menuX + 0.13f, 0.076f + 0.0575f, 255, 255, 255, 255, true, 0.3f, 0.3f); // Desenha o cabe�alho
}

// Vari�vel para o deslocamento do corpo do menu
float bodyOffset = 0;

// Fun��o para adicionar uma op��o ao menu
void addOption(const char* option, void (func)() = NULLVOID) {
    optionCount++; // Incrementa a contagem de op��es
    // Verifica se a op��o atual est� dentro dos limites do menu
    if (currentOption <= currentMenuMaxOptions && optionCount <= currentMenuMaxOptions && currentOption >= currentMenuMinOptions && optionCount >= currentMenuMinOptions) {
        // Desenha a op��o no menu
        draw_Text(option, menuX + 0.007f, 0.131f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 255, 255, 255, 255);
        drawRect(menuX, 0.124f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.260f, 0.038f, 0, 0, 0, 190);
        // Se a op��o atual for a que est� sendo desenhada, desenha um fundo especial
        if (currentOption == optionCount) {
            DrawSprite("generic_textures", "selection_box_bg_1d", menuX, 0.124f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.260f, 0.038f, 0, 255, 0, 0, 190);
            if (m_EnterKeyPressed) // Se a op��o foi pressionada, chama a fun��o associada
                func();
        }
    }
}

// Fun��o para adicionar uma op��o ao menu
void addUpdate(const char* text) {
    optionCount++; // Incrementa a contagem de op��es
    // Verifica se a op��o atual est� dentro dos limites do menu
    if (currentOption <= currentMenuMaxOptions && optionCount <= currentMenuMaxOptions && currentOption >= currentMenuMinOptions && optionCount >= currentMenuMinOptions) {
        // Desenha a op��o no menu
        draw_Text(text, menuX + 0.007f, 0.131f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 255, 255, 255, 255);
        drawRect(menuX, 0.124f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.260f, 0.038f, 0, 0, 0, 190);
        // Se a op��o atual for a que est� sendo desenhada, desenha um fundo especial
        if (currentOption == optionCount) {
            DrawSprite("generic_textures", "selection_box_bg_1d", menuX, 0.124f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.260f, 0.038f, 0, 255, 0, 0, 190);
        }
    }
}

// Fun��o para adicionar uma op��o que leva a outro submenu
void addSubmenuOption(const char* option, int submenu) {
    addOption(option); // Adiciona a op��o normalmente
    // Verifica se a op��o est� dentro dos limites do menu
    if (currentOption <= currentMenuMaxOptions && optionCount <= currentMenuMaxOptions && currentOption >= currentMenuMinOptions && optionCount >= currentMenuMinOptions) {
        // Desenha uma seta indicando que a op��o leva a outro submenu
        DrawSprite("menu_textures", "selection_arrow_right", menuX + 0.235f, 0.132f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.01125f, 0.02f, 0, 255, 255, 255, 255);
        // Se a op��o atual for a que est� sendo desenhada e foi pressionada, muda para o novo submenu
        if (currentOption == optionCount)
            if (m_EnterKeyPressed)
                changeSubmenu(submenu);
    }
}

// Fun��o sobrecarregada para adicionar uma op��o de submenu com uma fun��o associada
void addSubmenuOption(const char* option, int submenu, void (func)()) {
    addOption(option); // Adiciona a op��o normalmente
    // Verifica se a op��o est� dentro dos limites do menu
    if (currentOption <= currentMenuMaxOptions && optionCount <= currentMenuMaxOptions && currentOption >= currentMenuMinOptions && optionCount >= currentMenuMinOptions) {
        // Desenha uma seta indicando que a op��o leva a outro submenu
        DrawSprite("menu_textures", "selection_arrow_right", menuX + 0.235f, 0.132f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.01125f, 0.02f, 0, 255, 255, 255, 255);
        // Se a op��o atual for a que est� sendo desenhada
        if (currentOption == optionCount) {
            // Se a op��o foi pressionada, chama a fun��o e muda para o novo submenu
            if (m_EnterKeyPressed) {
                func(); // Chama a fun��o associada
                changeSubmenu(submenu); // Muda para o novo submenu
            }
        }
    }
}

// Fun��o para adicionar uma op��o booleana (checkbox) ao menu
void addBoolOption(const char* option, bool var, void (func)() = NULLVOID) {
    addOption(option); // Adiciona a op��o normalmente
    // Verifica se a op��o est� dentro dos limites do menu
    if (currentOption <= currentMenuMaxOptions && optionCount <= currentMenuMaxOptions && currentOption >= currentMenuMinOptions && optionCount >= currentMenuMinOptions) {
        // Desenha um s�mbolo de verifica��o se a op��o est� ativada
        if (var) {
            DrawSprite("generic_textures", "tick_box", menuX + 0.232f, 0.132f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.0140625f, 0.025f, 0, 255, 255, 255, 255);
            DrawSprite("generic_textures", "tick", menuX + 0.232f, 0.132f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.0140625f, 0.025f, 0, 255, 255, 255, 255);
        }
        else {
            // Se a op��o n�o est� ativada, apenas desenha a caixa de verifica��o
            DrawSprite("generic_textures", "tick_box", menuX + 0.232f, 0.132f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.0140625f, 0.025f, 0, 255, 255, 255, 255);
        }
        // Se a op��o atual for a que est� sendo desenhada e foi pressionada, chama a fun��o associada
        if (currentOption == optionCount)
            if (m_EnterKeyPressed)
                func();
    }
}

// Fun��o para adicionar uma op��o inteira (com controle de incremento/decremento) ao menu
void addIntOption(const char* option, int* var, int step = 1, bool fastPress = false, int min = -2147483647, int max = 2147483647) {
    char buffer[64]; // Buffer para formatar o texto da op��o
    snprintf(buffer, 64, "%s < %i >", option, *var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a op��o formatada
    if (currentOption == optionCount) { // Verifica se � a op��o atual
        // Se a press�o r�pida estiver ativada, muda o valor de acordo com a press�o
        if (fastPress) {
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao m�nimo, reseta para o m�ximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao m�ximo, reseta para o m�nimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        else
        {
            // Caso normal
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao m�nimo, reseta para o m�ximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao m�ximo, reseta para o m�nimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
    }
}

// Fun��o sobrecarregada para adicionar uma op��o inteira com uma fun��o associada
void addIntOption(const char* option, int* var, void (func)(), int step = 1, bool fastPress = false, int min = -2147483647, int max = 2147483647) {
    char buffer[64]; // Buffer para formatar o texto da op��o
    snprintf(buffer, 64, "%s < %i >", option, *var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a op��o formatada
    if (currentOption == optionCount) { // Verifica se � a op��o atual
        // Se a press�o r�pida estiver ativada, muda o valor de acordo com a press�o
        if (fastPress) {
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao m�nimo, reseta para o m�ximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao m�ximo, reseta para o m�nimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        else
        {
            // Caso normal
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao m�nimo, reseta para o m�ximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao m�ximo, reseta para o m�nimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        // Se a op��o foi pressionada, chama a fun��o associada
        if (m_EnterKeyPressed)
            func();
    }
}

// Fun��o para adicionar uma op��o de ponto flutuante ao menu
void addFloatOption(const char* option, float* var, float step, bool fastPress = false, float min = -3.4028235e38, float max = 3.4028235e38) {
    char buffer[64]; // Buffer para formatar o texto da op��o
    snprintf(buffer, 64, "%s < %.03f >", option, *var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a op��o formatada
    if (currentOption == optionCount) { // Verifica se � a op��o atual
        // Se a press�o r�pida estiver ativada, muda o valor de acordo com a press�o
        if (fastPress) {
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao m�nimo, reseta para o m�ximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao m�ximo, reseta para o m�nimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        else
        {
            // Caso normal
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao m�nimo, reseta para o m�ximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao m�ximo, reseta para o m�nimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
    }
}

// Fun��o sobrecarregada para adicionar uma op��o de ponto flutuante com uma fun��o associada
void addFloatOption(const char* option, float* var, float step, void (func)(), bool fastPress = false, float min = -3.4028235e38, float max = 3.4028235e38) {
    char buffer[64]; // Buffer para formatar o texto da op��o
    snprintf(buffer, 64, "%s < %.03f >", option, *var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a op��o formatada
    if (currentOption == optionCount) { // Verifica se � a op��o atual
        // Se a press�o r�pida estiver ativada, muda o valor de acordo com a press�o
        if (fastPress) {
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao m�nimo, reseta para o m�ximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao m�ximo, reseta para o m�nimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        else
        {
            // Caso normal
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao m�nimo, reseta para o m�ximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao m�ximo, reseta para o m�nimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        // Se a op��o foi pressionada, chama a fun��o associada
        if (m_EnterKeyPressed)
            func();
    }
}

// Fun��o para adicionar uma op��o vetorial com l�gica similar � fun��o de ponto flutuante
void addVectorOption(const char* option, int* var, int step, int min, int max, void (func)() = NULLVOID, bool fastPress = false) {
    char buffer[64]; // Buffer para formatar o texto da op��o
    snprintf(buffer, sizeof(buffer), "%s < %d >", option, *var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a op��o formatada

    if (currentOption == optionCount) { // Verifica se � a op��o atual
        // Se a press�o r�pida estiver ativada, ajusta conforme a entrada
        if (fastPress) {
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao m�nimo, reseta para o m�ximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor por step
                AdjustTime(); // Ajusta o tempo em tempo real
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao m�ximo, reseta para o m�nimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor por step
                AdjustTime(); // Ajusta o tempo em tempo real
            }
        }
        else {
            // Caso normal
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao m�nimo, reseta para o m�ximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor por step
                AdjustTime(); // Ajusta o tempo em tempo real
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao m�ximo, reseta para o m�nimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor por step
                AdjustTime(); // Ajusta o tempo em tempo real
            }
        }
        // Chama a fun��o associada, se a op��o for pressionada
        if (m_EnterKeyPressed && func != NULLVOID)
            func();
    }
}

// Fun��o para adicionar uma op��o de string ao menu
void addStringOption(const char* option, const char* var, int* intvar, int elementCount, bool fastPress = false) {
    char buffer[64]; // Buffer para formatar o texto da op��o
    snprintf(buffer, 64, "%s < %s >", option, var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a op��o formatada
    if (currentOption == optionCount) { // Verifica se � a op��o atual
        // Se a press�o r�pida n�o estiver ativada, muda o valor de acordo com a press�o
        if (fastPress == false)
        {
            if (m_LeftKeyPressed) {
                if (*intvar <= 0) // Se chegou ao primeiro elemento, reseta para o �ltimo
                    *intvar = elementCount;
                else
                    *intvar -= 1; // Decrementa o �ndice
            }
            else if (m_RightKeyPressed)
            {
                if (*intvar >= elementCount) // Se chegou ao �ltimo elemento, reseta para o primeiro
                    *intvar = 0;
                else
                    *intvar += 1; // Incrementa o �ndice
            }
        }
        else
        {
            // Se a press�o r�pida estiver ativada, muda o valor de acordo com a press�o
            if (m_LeftKeyPressed) {
                if (*intvar <= 0) // Se chegou ao primeiro elemento, reseta para o �ltimo
                    *intvar = elementCount;
                else
                    *intvar -= 1; // Decrementa o �ndice
            }
            else if (m_RightKeyPressed)
            {
                if (*intvar >= elementCount) // Se chegou ao �ltimo elemento, reseta para o primeiro
                    *intvar = 0;
                else
                    *intvar += 1; // Incrementa o �ndice
            }
        }
    }
}

// Fun��o sobrecarregada para adicionar uma op��o de string com uma fun��o associada
void addStringOption(const char* option, const char* var, int* intvar, int elementCount, void (func)(), bool fastPress = false) {
    char buffer[64]; // Buffer para formatar o texto da op��o
    snprintf(buffer, 64, "%s < %s >", option, var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a op��o formatada
    if (currentOption == optionCount) { // Verifica se � a op��o atual
        // Se a press�o r�pida n�o estiver ativada, muda o valor de acordo com a press�o
        if (fastPress == false)
        {
            if (m_LeftKeyPressed) {
                if (*intvar <= 0) // Se chegou ao primeiro elemento, reseta para o �ltimo
                    *intvar = elementCount;
                else
                    *intvar -= 1; // Decrementa o �ndice
            }
            else if (m_RightKeyPressed)
            {
                if (*intvar >= elementCount) // Se chegou ao �ltimo elemento, reseta para o primeiro
                    *intvar = 0;
                else
                    *intvar += 1; // Incrementa o �ndice
            }
        }
        else
        {
            // Se a press�o r�pida estiver ativada, muda o valor de acordo com a press�o
            if (m_LeftKeyPressed) {
                if (*intvar <= 0) // Se chegou ao primeiro elemento, reseta para o �ltimo
                    *intvar = elementCount;
                else
                    *intvar -= 1; // Decrementa o �ndice
            }
            else if (m_RightKeyPressed)
            {
                if (*intvar >= elementCount) // Se chegou ao �ltimo elemento, reseta para o primeiro
                    *intvar = 0;
                else
                    *intvar += 1; // Incrementa o �ndice
            }
        }
        // Se a op��o foi pressionada, chama a fun��o associada
        if (m_EnterKeyPressed)
            func();
    }
}

// Fun��o para exibir o �ndice da op��o atual no menu
void displayOptionIndex() {
    char buffer[32]; // Buffer para armazenar a string do �ndice
    snprintf(buffer, 32, "%i of %i", currentOption, optionCount); // Formata o �ndice atual e total
    // Se o n�mero de op��es � maior ou igual ao m�ximo permitido, desenha a informa��o no menu
    if (optionCount >= maxOptions) {
        draw_Text(buffer, menuX + 0.13f, 0.131f + (0.038f * (maxOptions + 1)), 255, 255, 255, 255, true);
        drawRect(menuX, 0.124f + (0.038f * (maxOptions + 1)), 0.260f, 0.038f, 0, 0, 0, 255); // Se voc� quiser que o ret�ngulo fique maior ou menor, modifique os valores 0.260f (largura) e 0.038f (altura).
        DrawSprite("generic_textures", "hud_menu_4a", menuX, 0.126f + (0.038f * (maxOptions + 1)), 0.260f, 0.002f, 0, 255, 255, 255, 255);
    }
    else {
        // Se n�o, desenha o �ndice com base no n�mero total de op��es
        draw_Text(buffer, menuX + 0.13f, 0.131f + (0.038f * (optionCount + 1)), 255, 255, 255, 255, true);
        drawRect(menuX, 0.124f + (0.038f * (optionCount + 1)), 0.260f, 0.038f, 0, 0, 0, 255); // Se voc� quiser que o ret�ngulo fique maior ou menor, modifique os valores 0.260f (largura) e 0.038f (altura)
        DrawSprite("generic_textures", "hud_menu_4a", menuX, 0.126f + (0.038f * (optionCount + 1)), 0.260f, 0.002f, 0, 255, 255, 255, 255);
    }
}

// Fun��o para resetar as vari�veis do menu
void resetVars()
{
    // Se o submenu n�o estiver fechado, exibe o �ndice da op��o
    if (submenu != Closed) {
        displayOptionIndex(); // Chama a fun��o para exibir o �ndice
    }

    // Reseta as flags de press�o de bot�o
    m_OpenKeyPressed = false;
    m_BackKeyPressed = false;
    m_EnterKeyPressed = false;
    m_UpKeyPressed = false;
    m_DownKeyPressed = false;
    m_LeftKeyPressed = false;
    m_RightKeyPressed = false;
    m_LMBPressed = false; // Reseta a flag de clique esquerdo do mouse
    menuHeader = false; // Reseta a flag do cabe�alho
}

// Fun��o que monitora os bot�es pressionados para navega��o no menu
void ButtonMonitoring()
{
    m_ControlIndex = PAD::IS_USING_KEYBOARD_AND_MOUSE(0) ? 0 : 2; // Definindo o �ndice de controle

    // Verifica os inputs
    m_OpenKeyPressed = IsKeyJustUp(VK_F7) || (PAD::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_RB) && PAD::IS_CONTROL_JUST_RELEASED(2, INPUT_CONTEXT_A));
    m_EnterKeyPressed = PAD::IS_CONTROL_JUST_RELEASED(m_ControlIndex, INPUT_GAME_MENU_ACCEPT);
    m_BackKeyPressed = PAD::IS_CONTROL_JUST_RELEASED(m_ControlIndex, INPUT_GAME_MENU_CANCEL);
    m_UpKeyPressed = PAD::IS_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_UP);
    m_DownKeyPressed = PAD::IS_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_DOWN);
    m_LeftKeyPressed = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_LEFT);
    m_RightKeyPressed = PAD::IS_DISABLED_CONTROL_JUST_PRESSED(m_ControlIndex, INPUT_GAME_MENU_RIGHT);

    if (submenu == Closed) // Se o menu estiver fechado
    {
        if (m_OpenKeyPressed)
        {
            // Abre o menu principal e reseta as vari�veis
            submenu = Main_Menu;
            submenuLevel = 0;
            currentOption = 1;
            currentMenuMinOptions = 1;
            currentMenuMaxOptions = maxOptions;
        }
    }
    else // Se o menu estiver aberto
    {
        // Desativa controles enquanto o menu estiver aberto
        if (m_ControlIndex == INPUT_GROUP_GAMEPAD) {
            *getGlobalPtr(1900383 + 316) = 2; // Desabilita o assobio do cavalo nesta frame
            PAD::DISABLE_CONTROL_ACTION(INPUT_GROUP_GAMEPAD, INPUT_WHISTLE, false);
            PAD::DISABLE_CONTROL_ACTION(INPUT_GROUP_GAMEPAD, INPUT_WHISTLE_HORSEBACK, false);
        }

        // Desativa os controles especificados
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_SELECT_RADAR_MODE, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_REVEAL_HUD, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_PLAYER_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_JOURNAL, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_SATCHEL_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_SATCHEL_HORSE_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_CRAFTING_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_EMOTE_WHEEL, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_OPEN_WHEEL_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_EXPAND_RADAR, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_INTERACTION_MENU, false);
        PAD::DISABLE_CONTROL_ACTION(m_ControlIndex, INPUT_HUD_SPECIAL, false);

        // Verifica se a tecla Enter foi pressionada
        if (m_EnterKeyPressed) {
            m_EnterKeyPressed = true; // Marca que uma op��o foi pressionada
        }

        // Verifica se a tecla de voltar foi pressionada
        if (m_BackKeyPressed) {
            if (submenu == Main_Menu) {
                CloseMenu(); // Fecha o menu se estiver no menu principal
            }
            else {
                // Retorna ao �ltimo submenu e restaura as op��es correspondentes
                submenu = lastSubmenu[submenuLevel - 1];
                currentOption = lastOption[submenuLevel - 1];
                currentMenuMinOptions = lastSubmenuMinOptions[submenuLevel - 1];
                currentMenuMaxOptions = lastSubmenuMaxOptions[submenuLevel - 1];
                submenuLevel--; // Decrementa o n�vel do submenu
            }
        }

        // Verifica se a tecla para cima foi pressionada
        if (m_UpKeyPressed) {
            if (currentOption == 1) {
                // Muda para a �ltima op��o
                currentOption = optionCount;
                currentMenuMaxOptions = optionCount;
                if (optionCount > maxOptions)
                    currentMenuMinOptions = optionCount - maxOptions + 1;
                else
                    currentMenuMinOptions = 1;
            }
            else {
                currentOption--;
                if (currentOption < currentMenuMinOptions) {
                    currentMenuMinOptions = currentOption;
                    currentMenuMaxOptions = currentOption + maxOptions - 1;
                }
            }
        }

        // Verifica se a tecla para baixo foi pressionada
        if (m_DownKeyPressed) {
            if (currentOption == optionCount) {
                // Muda para a primeira op��o
                currentOption = 1;
                currentMenuMinOptions = 1;
                currentMenuMaxOptions = maxOptions;
            }
            else {
                currentOption++;
                if (currentOption > currentMenuMaxOptions) {
                    currentMenuMaxOptions = currentOption;
                    currentMenuMinOptions = currentOption - maxOptions + 1;
                }
            }
        }

        // Verifica se a tecla para a esquerda foi pressionada
        if (m_LeftKeyPressed) {
            m_LeftKeyPressed = true; // Marca que a tecla esquerda foi pressionada
        }

        // Verifica se a tecla para a direita foi pressionada
        if (m_RightKeyPressed) {
            m_RightKeyPressed = true; // Marca que a tecla direita foi pressionada
        }
    }
}

/*
// Fun��o que monitora os bot�es pressionados para navega��o no menu
void ButtonMonitoring()
{
    if (submenu == Closed) // Se o menu estiver fechado
    {
        // Verifica se o bot�o para abrir o menu foi pressionado
        if (PAD::IS_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_LT")) || PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_LT")))
        {

            //PAD::IS_DISABLED_CONTROL_PRESSED(2, joaat("INPUT_FRONTEND_RB")) && PAD::IS_CONTROL_JUST_RELEASED(2, joaat("INPUT_CONTEXT_A"))

            // Abre o menu principal e reseta as vari�veis
            submenu = Main_Menu;
            submenuLevel = 0;
            currentOption = 1;
            currentMenuMinOptions = 1;
            currentMenuMaxOptions = maxOptions;
        }
    }
    else
    {
        // Verifica se a op��o de selecionar (Enter) foi pressionada
        if (PAD::IS_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_RDOWN"))) {
            optionPress = true; // Marca que uma op��o foi pressionada
        }

        // Verifica se a op��o de voltar (Backspace) foi pressionada
        if (PAD::IS_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_RRIGHT")))
        {
            if (submenu == Main_Menu) {
                CloseMenu(); // Fecha o menu se estiver no menu principal
            }
            else
            {
                // Retorna ao �ltimo submenu e restaura as op��es correspondentes
                submenu = lastSubmenu[submenuLevel - 1];
                currentOption = lastOption[submenuLevel - 1];
                currentMenuMinOptions = lastSubmenuMinOptions[submenuLevel - 1];
                currentMenuMaxOptions = lastSubmenuMaxOptions[submenuLevel - 1];
                submenuLevel--; // Decrementa o n�vel do submenu
            }
        }

        // Verifica se a op��o de rolar para cima foi pressionada
        if (PAD::IS_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_UP")))
        {
            if (currentOption == 1) // Se a op��o atual � a primeira
            {
                // Muda para a �ltima op��o
                currentOption = optionCount;
                currentMenuMaxOptions = optionCount;
                if (optionCount > maxOptions)
                    currentMenuMinOptions = optionCount - maxOptions + 1; // Ajusta a contagem m�nima
                else
                    currentMenuMinOptions = 1; // Se n�o, reseta para 1
            }
            else
            {
                currentOption--; // Decrementa a op��o atual
                // Ajusta as contagens m�nima e m�xima de op��es se necess�rio
                if (currentOption < currentMenuMinOptions) {
                    currentMenuMinOptions = currentOption;
                    currentMenuMaxOptions = currentOption + maxOptions - 1;
                }
            }
        }

        // Verifica se a op��o de rolar para baixo foi pressionada
        if (PAD::IS_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_DOWN")))
        {
            if (currentOption == optionCount) // Se a op��o atual � a �ltima
            {
                // Muda para a primeira op��o
                currentOption = 1;
                currentMenuMinOptions = 1;
                currentMenuMaxOptions = maxOptions;
            }
            else
            {
                currentOption++; // Incrementa a op��o atual
                // Ajusta as contagens m�nima e m�xima de op��es se necess�rio
                if (currentOption > currentMenuMaxOptions) {
                    currentMenuMaxOptions = currentOption;
                    currentMenuMinOptions = currentOption - maxOptions + 1;
                }
            }
        }

        // Verifica se a op��o de rolar para a esquerda foi pressionada
        if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_LEFT")))
        {
            leftPress = true; // Marca que a tecla esquerda foi pressionada
        }
        // Verifica se a op��o de rolar para a direita foi pressionada
        if (PAD::IS_DISABLED_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_RIGHT")))
        {
            rightPress = true; // Marca que a tecla direita foi pressionada
        }

        // Verifica se a tecla esquerda est� pressionada continuamente
        if (PAD::IS_DISABLED_CONTROL_PRESSED(0, joaat("INPUT_FRONTEND_LEFT"))) {
            fastLeftPress = true; // Marca que a tecla esquerda est� pressionada rapidamente
        }
        // Verifica se a tecla direita est� pressionada continuamente
        if (PAD::IS_DISABLED_CONTROL_PRESSED(0, joaat("INPUT_FRONTEND_RIGHT"))) {
            fastRightPress = true; // Marca que a tecla direita est� pressionada rapidamente
        }
    }
}
*/
