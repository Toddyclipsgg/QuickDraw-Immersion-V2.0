#pragma once


// Lista de fontes dispon�veis
const std::vector<const char*> FontList = { "body", "body1", "catalog1", "catalog2", "catalog3", "catalog4", "catalog5", "chalk",
"Debug_BOLD", "FixedWidthNumbers", "Font5", "gamername", "handwritten", "ledger", "RockstarTAG", "SOCIAL_CLUB_COND_BOLD", "title", "wantedPostersGeneric" };


// Fun��o que calcula o hash de uma string usando a fun��o JOAAT (algoritmo hash espec�fico do jogo)
Hash joaat(const char* string)
{
    return MISC::GET_HASH_KEY(string); // Usa a fun��o nativa para obter o hash da string fornecida
}

// Fun��o para desenhar uma sprite (imagem 2D) na tela
void DrawSprite(const char* category, const char* sprite, float x, float y, float scalex, float scaley, float rotation, int r, int g, int b, int a)
{
    // Calcula as coordenadas do centro da sprite
    float fX = x + scalex / 2;
    float fY = y + scaley / 2;

    // Verifica se a textura da sprite foi carregada, se n�o, solicita o carregamento
    if (!TXD::HAS_STREAMED_TEXTURE_DICT_LOADED(sprite))
        TXD::REQUEST_STREAMED_TEXTURE_DICT(sprite, 0);

    // Desenha a sprite na tela com os par�metros fornecidos
    GRAPHICS::DRAW_SPRITE(category, sprite, fX, fY, scalex, scaley, rotation, r, g, b, a, 1);

    // Libera a textura ap�s ser usada, marcando-a como n�o mais necess�ria
    TXD::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED(category);
}

// Fun��o para desenhar um texto na tela com fonte personalizada
void draw_TextWithFont(const char* text, float x, float y, int r, int g, int b, int a, int fontIndex, bool centered = false, float sx = 0.342f, float sy = 0.342f)
{
    // Define a cor do texto com base nos par�metros RGBA fornecidos
    UIDEBUG::_BG_SET_TEXT_COLOR(r, g, b, a);

    // Define a escala do texto (tamanho) com base nos valores de sx e sy
    UIDEBUG::_BG_SET_TEXT_SCALE(sx, sy);

    // Define a fonte com base no �ndice fornecido
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

// Fun��o para desenhar um texto na tela
void draw_Text(const char* text, float x, float y, int r, int g, int b, int a, bool centered = false, float sx = 0.342f, float sy = 0.342f)
{
    // Define a cor do texto com base nos par�metros RGBA fornecidos
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

// Fun��o para desenhar um ret�ngulo colorido na tela
void drawRect(float x, float y, float width, float height, int r, int g, int b, int a)
{
    // Calcula as coordenadas do centro do ret�ngulo
    float fX = x + width / 2;
    float fY = y + height / 2;

    // Desenha o ret�ngulo na tela com as dimens�es e cor especificadas
    GRAPHICS::DRAW_RECT(fX, fY, width, height, r, g, b, a, true, true);
}

// Fun��o para exibir uma legenda (subtitle) na tela
void PrintSubtitle(const char* text)
{
    // Converte o texto para um formato literal string (usado pelo jogo)
    const char* literalString = MISC::VAR_STRING(10, "LITERAL_STRING", text);

    // Exibe a legenda no log da interface do usu�rio (UILOG) do jogo
    UILOG::_UILOG_SET_CACHED_OBJECTIVE(literalString); // Adiciona a legenda
    UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();              // Atualiza o log
    UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();              // Exibe a legenda
}
