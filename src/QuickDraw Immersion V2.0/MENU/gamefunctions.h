#pragma once


// Lista de fontes disponíveis
const std::vector<const char*> FontList = { "body", "body1", "catalog1", "catalog2", "catalog3", "catalog4", "catalog5", "chalk",
"Debug_BOLD", "FixedWidthNumbers", "Font5", "gamername", "handwritten", "ledger", "RockstarTAG", "SOCIAL_CLUB_COND_BOLD", "title", "wantedPostersGeneric" };


// Função que calcula o hash de uma string usando a função JOAAT (algoritmo hash específico do jogo)
Hash joaat(const char* string)
{
    return MISC::GET_HASH_KEY(string); // Usa a função nativa para obter o hash da string fornecida
}

// Função para desenhar uma sprite (imagem 2D) na tela
void DrawSprite(const char* category, const char* sprite, float x, float y, float scalex, float scaley, float rotation, int r, int g, int b, int a)
{
    // Calcula as coordenadas do centro da sprite
    float fX = x + scalex / 2;
    float fY = y + scaley / 2;

    // Verifica se a textura da sprite foi carregada, se não, solicita o carregamento
    if (!TXD::HAS_STREAMED_TEXTURE_DICT_LOADED(sprite))
        TXD::REQUEST_STREAMED_TEXTURE_DICT(sprite, 0);

    // Desenha a sprite na tela com os parâmetros fornecidos
    GRAPHICS::DRAW_SPRITE(category, sprite, fX, fY, scalex, scaley, rotation, r, g, b, a, 1);

    // Libera a textura após ser usada, marcando-a como não mais necessária
    TXD::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED(category);
}

// Função para desenhar um texto na tela com fonte personalizada
void draw_TextWithFont(const char* text, float x, float y, int r, int g, int b, int a, int fontIndex, bool centered = false, float sx = 0.342f, float sy = 0.342f)
{
    // Define a cor do texto com base nos parâmetros RGBA fornecidos
    UIDEBUG::_BG_SET_TEXT_COLOR(r, g, b, a);

    // Define a escala do texto (tamanho) com base nos valores de sx e sy
    UIDEBUG::_BG_SET_TEXT_SCALE(sx, sy);

    // Define a fonte com base no índice fornecido
    const char* fontName = FontList[fontIndex];

    // Cria o texto formatado com a fonte selecionada
    std::string formatText = "<FONT FACE='$" + std::string(fontName) + "'>~s~" + std::string(text) + "</FONT>";

    // Define se o texto deve ser centralizado
    invoke<Void>(0xBE5261939FBECB8C, centered);

    // Converte o texto para um formato literal string (usado pelo jogo)
    const char* literalString = MISC::VAR_STRING(10, "LITERAL_STRING", formatText.c_str());

    // Desenha o texto nas coordenadas fornecidas (x, y)
    UIDEBUG::_BG_DISPLAY_TEXT(literalString, x, y);
}

// Função para desenhar um texto na tela
void draw_Text(const char* text, float x, float y, int r, int g, int b, int a, bool centered = false, float sx = 0.342f, float sy = 0.342f)
{
    // Define a cor do texto com base nos parâmetros RGBA fornecidos
    UIDEBUG::_BG_SET_TEXT_COLOR(r, g, b, a);

    // Define a escala do texto (tamanho) com base nos valores de sx e sy
    UIDEBUG::_BG_SET_TEXT_SCALE(sx, sy);

    // Define se o texto deve ser centralizado
    invoke<Void>(0xBE5261939FBECB8C, centered);

    // Converte o texto para um formato literal string (usado pelo jogo)
    const char* literalString = MISC::VAR_STRING(10, "LITERAL_STRING", text);

    // Desenha o texto nas coordenadas fornecidas (x, y)
    UIDEBUG::_BG_DISPLAY_TEXT(literalString, x, y);
}

// Função para desenhar um retângulo colorido na tela
void drawRect(float x, float y, float width, float height, int r, int g, int b, int a)
{
    // Calcula as coordenadas do centro do retângulo
    float fX = x + width / 2;
    float fY = y + height / 2;

    // Desenha o retângulo na tela com as dimensões e cor especificadas
    GRAPHICS::DRAW_RECT(fX, fY, width, height, r, g, b, a, true, true);
}

// Função para exibir uma legenda (subtitle) na tela
void PrintSubtitle(const char* text)
{
    // Converte o texto para um formato literal string (usado pelo jogo)
    const char* literalString = MISC::VAR_STRING(10, "LITERAL_STRING", text);

    // Exibe a legenda no log da interface do usuário (UILOG) do jogo
    UILOG::_UILOG_SET_CACHED_OBJECTIVE(literalString); // Adiciona a legenda
    UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();              // Atualiza o log
    UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();              // Exibe a legenda
}
