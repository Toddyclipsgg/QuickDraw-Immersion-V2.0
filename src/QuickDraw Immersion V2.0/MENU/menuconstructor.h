#pragma once

// Declaração de variáveis globais para controlar o estado do menu
int submenu = 0; // Armazena o submenu atual
int submenuLevel; // Nível atual do submenu (profundidade)
int lastSubmenu[20]; // Armazena o último submenu visitado em cada nível
int lastOption[20]; // Armazena a última opção selecionada em cada nível de submenu
int lastSubmenuMinOptions[20]; // Armazena a contagem mínima de opções no último submenu
int lastSubmenuMaxOptions[20]; // Armazena a contagem máxima de opções no último submenu
int currentOption; // Armazena a opção atualmente selecionada
int optionCount; // Contagem total de opções no submenu atual
int maxOptions = 8; // Número máximo de opções que podem ser exibidas no menu
int currentMenuMaxOptions = maxOptions; // Número máximo de opções no menu atual
int currentMenuMinOptions = 1; // Número mínimo de opções no menu atual
float menuX = 0.052f; // Posição horizontal do menu na tela
bool menuHeader = false; // Flag para indicar se há um cabeçalho no menu

// Variáveis de controle para inputs adicionais
int m_ControlIndex = INPUT_GROUP_KEYBOARD;
bool m_OpenKeyPressed = false;
bool m_BackKeyPressed = false;
bool m_EnterKeyPressed = false;
bool m_UpKeyPressed = false;
bool m_DownKeyPressed = false;
bool m_LeftKeyPressed = false;
bool m_RightKeyPressed = false;
bool m_LMBPressed = false;

// Função nula, que não faz nada. Usada como um valor padrão para funções.
void NULLVOID() {}

// Função que fecha o menu e reseta as variáveis relacionadas ao submenu
void CloseMenu()
{
    submenu = Closed; // Define o submenu como fechado
    submenuLevel = 0; // Reseta o nível do submenu
    currentOption = 1; // Reseta a opção selecionada para a primeira
}

// Função para mudar o submenu atual para um novo submenu
void changeSubmenu(int newSubmenu)
{
    // Armazena o submenu e a opção atuais antes de mudar
    lastSubmenu[submenuLevel] = submenu;
    lastOption[submenuLevel] = currentOption;
    lastSubmenuMinOptions[submenuLevel] = currentMenuMinOptions;
    lastSubmenuMaxOptions[submenuLevel] = currentMenuMaxOptions;

    // Reseta as variáveis do novo submenu
    currentOption = 1;
    currentMenuMinOptions = 1;
    currentMenuMaxOptions = maxOptions;
    submenu = newSubmenu; // Atualiza para o novo submenu
    submenuLevel++; // Aumenta o nível do submenu
    m_EnterKeyPressed = false; // Reseta a flag de pressão da opção
}

// Função para adicionar um título ao menu
void addTitle(const char* title) {
    optionCount = 0; // Reseta a contagem de opções para o título
    draw_TextWithFont(title, menuX + 0.13f, 0.076f, 255, 255, 255, 255, 16, true, 0.9f, 0.9f);
    // draw_Text(title, menuX + 0.13f, 0.076f, 255, 255, 255, 255, true, 0.5f, 0.5f); // Desenha o título
    drawRect(menuX, 0.050f, 0.260f, 0.112f, 0, 0, 0, 255); // Desenha o fundo do título
    // Desenha sprites específicas para o cabeçalho do menu
    // DrawSprite("honor_display", "honor_bad", menuX, 0.058f, 0.260f, 0.074f, 0, 255, 255, 255, 255);
    DrawSprite("generic_textures", "hud_menu_4a", menuX, 0.131f + 0.027f, 0.260f, 0.002f, 0, 255, 255, 255, 255);
}

// Função para adicionar um cabeçalho ao menu
void addHeader(const char* header)
{
    menuHeader = true; // Marca que o cabeçalho está ativo
    draw_Text(header, menuX + 0.13f, 0.076f + 0.0575f, 255, 255, 255, 255, true, 0.3f, 0.3f); // Desenha o cabeçalho
}

// Variável para o deslocamento do corpo do menu
float bodyOffset = 0;

// Função para adicionar uma opção ao menu
void addOption(const char* option, void (func)() = NULLVOID) {
    optionCount++; // Incrementa a contagem de opções
    // Verifica se a opção atual está dentro dos limites do menu
    if (currentOption <= currentMenuMaxOptions && optionCount <= currentMenuMaxOptions && currentOption >= currentMenuMinOptions && optionCount >= currentMenuMinOptions) {
        // Desenha a opção no menu
        draw_Text(option, menuX + 0.007f, 0.131f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 255, 255, 255, 255);
        drawRect(menuX, 0.124f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.260f, 0.038f, 0, 0, 0, 190);
        // Se a opção atual for a que está sendo desenhada, desenha um fundo especial
        if (currentOption == optionCount) {
            DrawSprite("generic_textures", "selection_box_bg_1d", menuX, 0.124f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.260f, 0.038f, 0, 255, 0, 0, 190);
            if (m_EnterKeyPressed) // Se a opção foi pressionada, chama a função associada
                func();
        }
    }
}

// Variável de tempo global para controlar a pulsação
float pulseTimer = 0.0f;

// Função para adicionar uma opção ao menu
void addOptionPulse(const char* option, void (*func)() = NULLVOID) {
    optionCount++; // Incrementa a contagem de opções

    // Verifica se a opção atual está dentro dos limites do menu
    if (currentOption <= currentMenuMaxOptions && optionCount <= currentMenuMaxOptions && currentOption >= currentMenuMinOptions && optionCount >= currentMenuMinOptions) {
        // Calcula a pulsação (alfa vai oscilar entre 50 e 150 para opções não selecionadas)
        int nonSelectedAlpha = static_cast<int>(50 + 100 * (sin(pulseTimer) * 0.5f + 0.5f));

        // Incrementa o tempo da pulsação para o próximo frame
        pulseTimer += 0.05f;
        if (pulseTimer > 6.28f) // Reseta o timer para manter dentro do intervalo de 2*PI
            pulseTimer = 0.0f;

        // Se a opção atual for a que está sendo desenhada
        if (currentOption == optionCount) {
            // Desenha o texto sem pulsar (opacidade total)
            draw_Text(option, menuX + 0.007f, 0.131f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 255, 255, 255, 255);
            drawRect(menuX, 0.124f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.260f, 0.038f, 0, 0, 0, 190);

            // Calcula a pulsação mais intensa (alfa vai oscilar entre 100 e 255)
            int selectedAlpha = static_cast<int>(100 + 155 * (sin(pulseTimer) * 0.5f + 0.5f));

            // Desenha o fundo com pulsação na cor (vermelho) com alfa variando
            DrawSprite("generic_textures", "selection_box_bg_1d", menuX, 0.124f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.260f, 0.038f, 0, 255, 0, 0, selectedAlpha);

            // Se a tecla Enter foi pressionada, executa a função associada
            if (m_EnterKeyPressed) {
                func();
            }
        }
        else {
            // Para as opções não selecionadas, o texto ainda pulsa
            draw_Text(option, menuX + 0.007f, 0.131f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 255, 255, 255, nonSelectedAlpha);
            drawRect(menuX, 0.124f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.260f, 0.038f, 0, 0, 0, 190);
        }
    }
}

// Função para adicionar uma opção ao menu
void addUpdate(const char* text) {
    optionCount++; // Incrementa a contagem de opções
    // Verifica se a opção atual está dentro dos limites do menu
    if (currentOption <= currentMenuMaxOptions && optionCount <= currentMenuMaxOptions && currentOption >= currentMenuMinOptions && optionCount >= currentMenuMinOptions) {
        // Desenha a opção no menu
        draw_Text(text, menuX + 0.007f, 0.131f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 255, 255, 255, 255);
        drawRect(menuX, 0.124f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.260f, 0.038f, 0, 0, 0, 190);
        // Se a opção atual for a que está sendo desenhada, desenha um fundo especial
        if (currentOption == optionCount) {
            DrawSprite("generic_textures", "selection_box_bg_1d", menuX, 0.124f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.260f, 0.038f, 0, 255, 0, 0, 190);
        }
    }
}

// Função para adicionar uma opção que leva a outro submenu
void addSubmenuOption(const char* option, int submenu) {
    addOption(option); // Adiciona a opção normalmente
    // Verifica se a opção está dentro dos limites do menu
    if (currentOption <= currentMenuMaxOptions && optionCount <= currentMenuMaxOptions && currentOption >= currentMenuMinOptions && optionCount >= currentMenuMinOptions) {
        // Desenha uma seta indicando que a opção leva a outro submenu
        DrawSprite("menu_textures", "selection_arrow_right", menuX + 0.235f, 0.132f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.01125f, 0.02f, 0, 255, 255, 255, 255);
        // Se a opção atual for a que está sendo desenhada e foi pressionada, muda para o novo submenu
        if (currentOption == optionCount)
            if (m_EnterKeyPressed)
                changeSubmenu(submenu);
    }
}

// Função sobrecarregada para adicionar uma opção de submenu com uma função associada
void addSubmenuOption(const char* option, int submenu, void (func)()) {
    addOption(option); // Adiciona a opção normalmente
    // Verifica se a opção está dentro dos limites do menu
    if (currentOption <= currentMenuMaxOptions && optionCount <= currentMenuMaxOptions && currentOption >= currentMenuMinOptions && optionCount >= currentMenuMinOptions) {
        // Desenha uma seta indicando que a opção leva a outro submenu
        DrawSprite("menu_textures", "selection_arrow_right", menuX + 0.235f, 0.132f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.01125f, 0.02f, 0, 255, 255, 255, 255);
        // Se a opção atual for a que está sendo desenhada
        if (currentOption == optionCount) {
            // Se a opção foi pressionada, chama a função e muda para o novo submenu
            if (m_EnterKeyPressed) {
                func(); // Chama a função associada
                changeSubmenu(submenu); // Muda para o novo submenu
            }
        }
    }
}

// Função para adicionar uma opção booleana (checkbox) ao menu
void addBoolOption(const char* option, bool var, void (func)() = NULLVOID) {
    addOption(option); // Adiciona a opção normalmente
    // Verifica se a opção está dentro dos limites do menu
    if (currentOption <= currentMenuMaxOptions && optionCount <= currentMenuMaxOptions && currentOption >= currentMenuMinOptions && optionCount >= currentMenuMinOptions) {
        // Desenha um símbolo de verificação se a opção está ativada
        if (var) {
            DrawSprite("generic_textures", "tick_box", menuX + 0.232f, 0.132f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.0140625f, 0.025f, 0, 255, 255, 255, 255);
            DrawSprite("generic_textures", "tick", menuX + 0.232f, 0.132f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.0140625f, 0.025f, 0, 255, 255, 255, 255);
        }
        else {
            // Se a opção não está ativada, apenas desenha a caixa de verificação
            DrawSprite("generic_textures", "tick_box", menuX + 0.232f, 0.132f + (0.038f * ((optionCount - currentMenuMinOptions) + 1)), 0.0140625f, 0.025f, 0, 255, 255, 255, 255);
        }
        // Se a opção atual for a que está sendo desenhada e foi pressionada, chama a função associada
        if (currentOption == optionCount)
            if (m_EnterKeyPressed)
                func();
    }
}

// Função para adicionar uma opção inteira (com controle de incremento/decremento) ao menu
void addIntOption(const char* option, int* var, int step = 1, bool fastPress = false, int min = -2147483647, int max = 2147483647) {
    char buffer[64]; // Buffer para formatar o texto da opção
    snprintf(buffer, 64, "%s < %i >", option, *var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a opção formatada
    if (currentOption == optionCount) { // Verifica se é a opção atual
        // Se a pressão rápida estiver ativada, muda o valor de acordo com a pressão
        if (fastPress) {
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao mínimo, reseta para o máximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao máximo, reseta para o mínimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        else
        {
            // Caso normal
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao mínimo, reseta para o máximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao máximo, reseta para o mínimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
    }
}

// Função sobrecarregada para adicionar uma opção inteira com uma função associada
void addIntOption(const char* option, int* var, void (func)(), int step = 1, bool fastPress = false, int min = -2147483647, int max = 2147483647) {
    char buffer[64]; // Buffer para formatar o texto da opção
    snprintf(buffer, 64, "%s < %i >", option, *var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a opção formatada
    if (currentOption == optionCount) { // Verifica se é a opção atual
        // Se a pressão rápida estiver ativada, muda o valor de acordo com a pressão
        if (fastPress) {
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao mínimo, reseta para o máximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao máximo, reseta para o mínimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        else
        {
            // Caso normal
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao mínimo, reseta para o máximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao máximo, reseta para o mínimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        // Se a opção foi pressionada, chama a função associada
        if (m_EnterKeyPressed)
            func();
    }
}

// Função para adicionar uma opção de ponto flutuante ao menu
void addFloatOption(const char* option, float* var, float step, bool fastPress = false, float min = -3.4028235e38, float max = 3.4028235e38) {
    char buffer[64]; // Buffer para formatar o texto da opção
    snprintf(buffer, 64, "%s < %.03f >", option, *var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a opção formatada
    if (currentOption == optionCount) { // Verifica se é a opção atual
        // Se a pressão rápida estiver ativada, muda o valor de acordo com a pressão
        if (fastPress) {
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao mínimo, reseta para o máximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao máximo, reseta para o mínimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        else
        {
            // Caso normal
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao mínimo, reseta para o máximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao máximo, reseta para o mínimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
    }
}

// Função sobrecarregada para adicionar uma opção de ponto flutuante com uma função associada
void addFloatOption(const char* option, float* var, float step, void (func)(), bool fastPress = false, float min = -3.4028235e38, float max = 3.4028235e38) {
    char buffer[64]; // Buffer para formatar o texto da opção
    snprintf(buffer, 64, "%s < %.03f >", option, *var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a opção formatada
    if (currentOption == optionCount) { // Verifica se é a opção atual
        // Se a pressão rápida estiver ativada, muda o valor de acordo com a pressão
        if (fastPress) {
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao mínimo, reseta para o máximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao máximo, reseta para o mínimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        else
        {
            // Caso normal
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao mínimo, reseta para o máximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao máximo, reseta para o mínimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor
            }
        }
        // Se a opção foi pressionada, chama a função associada
        if (m_EnterKeyPressed)
            func();
    }
}

// Função para adicionar uma opção vetorial com lógica similar à função de ponto flutuante
void addVectorOption(const char* option, int* var, int step, int min, int max, void (func)() = NULLVOID, bool fastPress = false) {
    char buffer[64]; // Buffer para formatar o texto da opção
    snprintf(buffer, sizeof(buffer), "%s < %d >", option, *var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a opção formatada

    if (currentOption == optionCount) { // Verifica se é a opção atual
        // Se a pressão rápida estiver ativada, ajusta conforme a entrada
        if (fastPress) {
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao mínimo, reseta para o máximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor por step
                AdjustTime(); // Ajusta o tempo em tempo real
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao máximo, reseta para o mínimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor por step
                AdjustTime(); // Ajusta o tempo em tempo real
            }
        }
        else {
            // Caso normal
            if (m_LeftKeyPressed) {
                if (*var == min) // Se chegou ao mínimo, reseta para o máximo
                    *var = max;
                else
                    *var -= step; // Decrementa o valor por step
                AdjustTime(); // Ajusta o tempo em tempo real
            }
            else if (m_RightKeyPressed) {
                if (*var == max) // Se chegou ao máximo, reseta para o mínimo
                    *var = min;
                else
                    *var += step; // Incrementa o valor por step
                AdjustTime(); // Ajusta o tempo em tempo real
            }
        }
        // Chama a função associada, se a opção for pressionada
        if (m_EnterKeyPressed && func != NULLVOID)
            func();
    }
}

// Função para adicionar uma opção de string ao menu
void addStringOption(const char* option, const char* var, int* intvar, int elementCount, bool fastPress = false) {
    char buffer[64]; // Buffer para formatar o texto da opção
    snprintf(buffer, 64, "%s < %s >", option, var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a opção formatada
    if (currentOption == optionCount) { // Verifica se é a opção atual
        // Se a pressão rápida não estiver ativada, muda o valor de acordo com a pressão
        if (fastPress == false)
        {
            if (m_LeftKeyPressed) {
                if (*intvar <= 0) // Se chegou ao primeiro elemento, reseta para o último
                    *intvar = elementCount;
                else
                    *intvar -= 1; // Decrementa o índice
            }
            else if (m_RightKeyPressed)
            {
                if (*intvar >= elementCount) // Se chegou ao último elemento, reseta para o primeiro
                    *intvar = 0;
                else
                    *intvar += 1; // Incrementa o índice
            }
        }
        else
        {
            // Se a pressão rápida estiver ativada, muda o valor de acordo com a pressão
            if (m_LeftKeyPressed) {
                if (*intvar <= 0) // Se chegou ao primeiro elemento, reseta para o último
                    *intvar = elementCount;
                else
                    *intvar -= 1; // Decrementa o índice
            }
            else if (m_RightKeyPressed)
            {
                if (*intvar >= elementCount) // Se chegou ao último elemento, reseta para o primeiro
                    *intvar = 0;
                else
                    *intvar += 1; // Incrementa o índice
            }
        }
    }
}

// Função sobrecarregada para adicionar uma opção de string com uma função associada
void addStringOption(const char* option, const char* var, int* intvar, int elementCount, void (func)(), bool fastPress = false) {
    char buffer[64]; // Buffer para formatar o texto da opção
    snprintf(buffer, 64, "%s < %s >", option, var); // Formata a string para mostrar o valor atual
    addOption(buffer); // Adiciona a opção formatada
    if (currentOption == optionCount) { // Verifica se é a opção atual
        // Se a pressão rápida não estiver ativada, muda o valor de acordo com a pressão
        if (fastPress == false)
        {
            if (m_LeftKeyPressed) {
                if (*intvar <= 0) // Se chegou ao primeiro elemento, reseta para o último
                    *intvar = elementCount;
                else
                    *intvar -= 1; // Decrementa o índice
            }
            else if (m_RightKeyPressed)
            {
                if (*intvar >= elementCount) // Se chegou ao último elemento, reseta para o primeiro
                    *intvar = 0;
                else
                    *intvar += 1; // Incrementa o índice
            }
        }
        else
        {
            // Se a pressão rápida estiver ativada, muda o valor de acordo com a pressão
            if (m_LeftKeyPressed) {
                if (*intvar <= 0) // Se chegou ao primeiro elemento, reseta para o último
                    *intvar = elementCount;
                else
                    *intvar -= 1; // Decrementa o índice
            }
            else if (m_RightKeyPressed)
            {
                if (*intvar >= elementCount) // Se chegou ao último elemento, reseta para o primeiro
                    *intvar = 0;
                else
                    *intvar += 1; // Incrementa o índice
            }
        }
        // Se a opção foi pressionada, chama a função associada
        if (m_EnterKeyPressed)
            func();
    }
}

// Função para exibir o índice da opção atual no menu
void displayOptionIndex() {
    char buffer[32]; // Buffer para armazenar a string do índice
    snprintf(buffer, 32, "%i of %i", currentOption, optionCount); // Formata o índice atual e total
    // Se o número de opções é maior ou igual ao máximo permitido, desenha a informação no menu
    if (optionCount >= maxOptions) {
        draw_Text(buffer, menuX + 0.13f, 0.131f + (0.038f * (maxOptions + 1)), 255, 255, 255, 255, true);
        drawRect(menuX, 0.124f + (0.038f * (maxOptions + 1)), 0.260f, 0.038f, 0, 0, 0, 255); // Se você quiser que o retângulo fique maior ou menor, modifique os valores 0.260f (largura) e 0.038f (altura).
        DrawSprite("generic_textures", "hud_menu_4a", menuX, 0.126f + (0.038f * (maxOptions + 1)), 0.260f, 0.002f, 0, 255, 255, 255, 255);
    }
    else {
        // Se não, desenha o índice com base no número total de opções
        draw_Text(buffer, menuX + 0.13f, 0.131f + (0.038f * (optionCount + 1)), 255, 255, 255, 255, true);
        drawRect(menuX, 0.124f + (0.038f * (optionCount + 1)), 0.260f, 0.038f, 0, 0, 0, 255); // Se você quiser que o retângulo fique maior ou menor, modifique os valores 0.260f (largura) e 0.038f (altura)
        DrawSprite("generic_textures", "hud_menu_4a", menuX, 0.126f + (0.038f * (optionCount + 1)), 0.260f, 0.002f, 0, 255, 255, 255, 255);
    }
}

// Função para resetar as variáveis do menu
void resetVars()
{
    // Se o submenu não estiver fechado, exibe o índice da opção
    if (submenu != Closed) {
        displayOptionIndex(); // Chama a função para exibir o índice
    }

    // Reseta as flags de pressão de botão
    m_OpenKeyPressed = false;
    m_BackKeyPressed = false;
    m_EnterKeyPressed = false;
    m_UpKeyPressed = false;
    m_DownKeyPressed = false;
    m_LeftKeyPressed = false;
    m_RightKeyPressed = false;
    m_LMBPressed = false; // Reseta a flag de clique esquerdo do mouse
    menuHeader = false; // Reseta a flag do cabeçalho
}

// Função que monitora os botões pressionados para navegação no menu
void ButtonMonitoring()
{
    m_ControlIndex = PAD::IS_USING_KEYBOARD_AND_MOUSE(0) ? 0 : 2; // Definindo o índice de controle

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
            // Abre o menu principal e reseta as variáveis
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
            m_EnterKeyPressed = true; // Marca que uma opção foi pressionada
        }

        // Verifica se a tecla de voltar foi pressionada
        if (m_BackKeyPressed) {
            if (submenu == Main_Menu) {
                CloseMenu(); // Fecha o menu se estiver no menu principal
            }
            else {
                // Retorna ao último submenu e restaura as opções correspondentes
                submenu = lastSubmenu[submenuLevel - 1];
                currentOption = lastOption[submenuLevel - 1];
                currentMenuMinOptions = lastSubmenuMinOptions[submenuLevel - 1];
                currentMenuMaxOptions = lastSubmenuMaxOptions[submenuLevel - 1];
                submenuLevel--; // Decrementa o nível do submenu
            }
        }

        // Verifica se a tecla para cima foi pressionada
        if (m_UpKeyPressed) {
            if (currentOption == 1) {
                // Muda para a última opção
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
                // Muda para a primeira opção
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