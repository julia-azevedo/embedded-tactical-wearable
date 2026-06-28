#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <stdlib.h>
#include "tatico_settings.h" /* Importa a interface do módulo de memória */

/* Função que será executada ao acionar o comando no terminal */
static int cmd_set_id(const struct shell *shell, size_t argc, char **argv)
{
    if (argc < 2) {
        shell_print(shell, "Erro: Informe o ID do soldado. Ex: tatico set_id x");
        return -EINVAL;
    }

    int novo_id = atoi(argv[1]);
    shell_print(shell, "Processando... Novo ID solicitado: %d", novo_id);

    /* Chama a interface pública do Módulo de Configurações para salvar */
    int rc = tatico_settings_save_id(novo_id);
    if (rc) {
        shell_print(shell, "Erro ao salvar na memoria flash: %d", rc);
    } else {
        shell_print(shell, "Memoria atualizada com sucesso!");
    }

    return 0;
}

/* Registro do comando no SO. 
   Hierarquia: tatico -> set_id -> [funcao]
*/
SHELL_STATIC_SUBCMD_SET_CREATE(sub_tatico,
    SHELL_CMD(set_id, NULL, "Configura o ID do soldado. Ex: tatico set_id x", cmd_set_id),
    SHELL_SUBCMD_SET_END /* Terminador obrigatório!! */
);
SHELL_CMD_REGISTER(tatico, &sub_tatico, "Comandos do dispositivo tático", NULL);