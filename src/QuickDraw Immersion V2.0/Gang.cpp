#include "header.h"

void RefatorarCodigo(int entidade, int* arma, float* parametro)
{
    // Título: RefatorarCodigo
    // Descrição: Atualiza parâmetros com base no modelo da entidade e arma fornecida.

    // Verifica se a arma está excluída
    if (ArmaExcluida(*arma))
    {
        *parametro = 0.0f; // Define o parâmetro como 0.0 se a arma for excluída
        return;
    }

    bool configurarPed = true;
    // Obtém o modelo da entidade
    int modeloEntidade = ENTITY::GET_ENTITY_MODEL(entidade);

    // Calcula o valor do parâmetro com base no modelo da entidade
    *parametro = ObterValorParametro(modeloEntidade);

    // Verifica se o modelo da entidade pertence à gangue "Dutch's"
    if (modeloEntidade == joaat("REL_GANG_DUTCHS"))
    {
        configurarPed = false; // Define para não configurar o ped se for "Dutch's"
    }

    // Configura a flag do ped se necessário
    if (configurarPed)
    {
        PED::SET_PED_CONFIG_FLAG(entidade, 361 /* PCF_IgnoreWeaponDegradation */, true);
    }
}

// Título: ArmaExcluida
// Descrição: Verifica se uma arma está excluída com base no seu ID.

bool ArmaExcluida(int arma)
{
    // Lista de armas excluídas
    int armasExcluidas[] = {
        joaat("WEAPON_REVOLVER_CATTLEMAN_MEXICAN"),
        joaat("WEAPON_REVOLVER_CATTLEMAN_PIG"),
        joaat("WEAPON_REVOLVER_DOUBLEACTION_EXOTIC"),
        joaat("WEAPON_REVOLVER_SCHOFIELD_CALLOWAY"),
        joaat("WEAPON_REVOLVER_SCHOFIELD_GOLDEN"),
        joaat("WEAPON_PISTOL_MAUSER_DRUNK"),
        joaat("WEAPON_SHOTGUN_DOUBLEBARREL_EXOTIC"),
        joaat("WEAPON_SNIPERRIFLE_ROLLINGBLOCK_EXOTIC")
    };

    // Calcula o número de armas excluídas
    int tamanho = sizeof(armasExcluidas) / sizeof(armasExcluidas[0]);

    // Verifica se a arma fornecida está na lista de armas excluídas
    for (int i = 0; i < tamanho; i++)
    {
        if (arma == armasExcluidas[i])
        {
            return true; // A arma está excluída
        }
    }
    return false; // A arma não está excluída
}

// Título: ObterValorParametro
float ObterValorParametro(int modelo)
{
    // Verifica se o modelo pertence ao grupo Duster
    if (ModeloNoGrupo(modelo, GrupoDuster(), sizeof(GrupoDuster()) / sizeof(GrupoDuster()[0])))
    {
        // Retorna um valor aleatório entre 0.3 e 0.5
        return MISC::GET_RANDOM_FLOAT_IN_RANGE(0.3f, 0.5f);
    }
    // Verifica se o modelo pertence ao grupo Ex-Confederados
    else if (ModeloNoGrupo(modelo, GrupoExConfederados(), sizeof(GrupoExConfederados()) / sizeof(GrupoExConfederados()[0])))
    {
        // Retorna um valor aleatório entre 0.125 e 0.375
        return MISC::GET_RANDOM_FLOAT_IN_RANGE(0.125f, 0.375f);
    }
    // Verifica se o modelo é "G_M_M_UNIINBRED_01"
    else if (modelo == joaat("G_M_M_UNIINBRED_01"))
    {
        // Retorna um valor aleatório entre 0.7 e 0.875
        return MISC::GET_RANDOM_FLOAT_IN_RANGE(0.7f, 0.875f);
    }
    // Verifica se o modelo pertence ao grupo Skinner Brothers
    else if (ModeloNoGrupo(modelo, GrupoSkinnerBrothers(), sizeof(GrupoSkinnerBrothers()) / sizeof(GrupoSkinnerBrothers()[0])))
    {
        // Retorna um valor aleatório entre 0.4 e 0.6
        return MISC::GET_RANDOM_FLOAT_IN_RANGE(0.4f, 0.6f);
    }
    // Verifica se o modelo pertence ao grupo Ranchers
    else if (ModeloNoGrupo(modelo, GrupoRanchers(), sizeof(GrupoRanchers()) / sizeof(GrupoRanchers()[0])))
    {
        // Retorna um valor aleatório entre 0.125 e 0.25
        return MISC::GET_RANDOM_FLOAT_IN_RANGE(0.125f, 0.25f);
    }
    // Verifica se o modelo pertence ao grupo Banditos
    else if (ModeloNoGrupo(modelo, GrupoBanditos(), sizeof(GrupoBanditos()) / sizeof(GrupoBanditos()[0])))
    {
        // Retorna um valor aleatório entre 0.3 e 0.5
        return MISC::GET_RANDOM_FLOAT_IN_RANGE(0.3f, 0.5f);
    }
    // Verifica se o modelo pertence ao grupo Bronte
    else if (ModeloNoGrupo(modelo, GrupoBronte(), sizeof(GrupoBronte()) / sizeof(GrupoBronte()[0])))
    {
        // Retorna um valor fixo de 0.125
        return MISC::GET_RANDOM_FLOAT_IN_RANGE(0.125f, 0.125f);
    }
    else
    {
        // Retorna um valor aleatório entre 0.3 e 0.4 para outros modelos
        return MISC::GET_RANDOM_FLOAT_IN_RANGE(0.3f, 0.4f);
    }
}

// Título: ModeloNoGrupo
// Descrição: Verifica se um modelo está presente em um grupo de modelos.

bool ModeloNoGrupo(int modelo, int* grupoModelos, int tamanhoGrupo)
{
    // Itera sobre todos os modelos no grupo
    for (int i = 0; i < tamanhoGrupo; i++)
    {
        // Verifica se o modelo atual é igual ao modelo fornecido
        if (modelo == grupoModelos[i])
        {
            return true; // Modelo encontrado no grupo
        }
    }
    return false; // Modelo não encontrado no grupo
}

// Título: GrupoDuster
// Descrição: Retorna um array estático contendo os modelos pertencentes ao grupo Duster.

int* GrupoDuster()
{
    // Array estático de modelos do grupo Duster
    static int modelos[] = {
        joaat("G_M_M_UNIDUSTER_03"),
        joaat("U_M_M_UNIDUSTERHENCHMAN_03"),
        joaat("G_M_M_UNIDUSTER_04"),
        joaat("U_M_M_UNIDUSTERHENCHMAN_02"),
        joaat("G_M_M_UNIDUSTER_02"),
        joaat("U_M_M_FATDUSTER_01"),
        joaat("U_M_M_UNIDUSTERHENCHMAN_01"),
        joaat("G_M_M_UNIDUSTER_01"),
        joaat("CS_DISGUISEDDUSTER_01"),
        joaat("G_F_M_UNIDUSTER_01"),
        joaat("U_M_M_UNIDUSTERLEADER_01"),
        joaat("CS_DISGUISEDDUSTER_03"),
        joaat("G_M_M_UNIDUSTER_05"),
        joaat("CS_DISGUISEDDUSTER_02")
    };
    return modelos; // Retorna o array de modelos do grupo Duster
}

// Título: GrupoExConfederados
int* GrupoExConfederados()
{
    // Descrição: Retorna um array estático contendo os modelos pertencentes ao grupo Ex-Confederados.

    // Array estático de modelos do grupo Ex-Confederados
    static int modelos[] = {
        joaat("G_M_Y_UNIEXCONFEDS_02"),
        joaat("G_M_Y_UNIEXCONFEDS_01"),
        joaat("G_M_O_UNIEXCONFEDS_01"),
        joaat("CS_EXCONFEDSLEADER_01"),
        joaat("U_M_M_UNIEXCONFEDSBOUNTY_01")
    };

    // Retorna o array de modelos do grupo Ex-Confederados
    return modelos;
}

// Título: GrupoSkinnerBrothers
int* GrupoSkinnerBrothers()
{
    // Descrição: Retorna um array estático contendo os modelos pertencentes ao grupo Skinner Brothers.

    // Array estático de modelos do grupo Skinner Brothers
    static int modelos[] = {
        joaat("U_M_M_BHT_SKINNERSEARCH"),
        joaat("G_M_M_UNIMOUNTAINMEN_01"),
        joaat("U_M_M_BHT_SKINNERBROTHER")
    };

    // Retorna o array de modelos do grupo Skinner Brothers
    return modelos;
}

// Título: GrupoRanchers
int* GrupoRanchers()
{
    // Descrição: Retorna um array estático contendo os modelos pertencentes ao grupo Ranchers.

    // Array estático de modelos do grupo Ranchers
    static int modelos[] = {
        joaat("G_M_M_UNIRANCHERS_01"),
        joaat("CS_LARAMIE")
    };

    // Retorna o array de modelos do grupo Ranchers
    return modelos;
}

// Título: GrupoBanditos
int* GrupoBanditos()
{
    // Descrição: Retorna um array estático contendo os modelos pertencentes ao grupo Banditos.

    // Array estático de modelos do grupo Banditos
    static int modelos[] = {
        joaat("U_M_M_BHT_BANDITOSHACK"),
        joaat("U_M_M_BHT_BANDITOMINE"),
        joaat("G_M_M_UNIBANDITOS_01")
    };

    // Retorna o array de modelos do grupo Banditos
    return modelos;
}

// Título: GrupoBronte
int* GrupoBronte()
{
    // Descrição: Retorna um array estático contendo os modelos pertencentes ao grupo Bronte.

    // Array estático de modelos do grupo Bronte
    static int modelos[] = {
        joaat("U_M_M_NBXBRONTEGOON_01"),
        joaat("U_M_M_NBXBRONTESECFORM_01"),
        joaat("U_M_M_NBXBRONTEASC_01"),
        joaat("G_M_M_UNIBRONTEGOONS_01")
    };

    // Retorna o array de modelos do grupo Bronte
    return modelos;
}
