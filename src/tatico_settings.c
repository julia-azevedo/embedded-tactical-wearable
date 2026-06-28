#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>
#include "tatico_settings.h"

/* Registo do módulo de log exclusivo para a gestão de memória */
LOG_MODULE_REGISTER(tatico_settings, LOG_LEVEL_INF);

/* Variável estática (privada): nenhum outro arquivo consegue alterá-la diretamente */
static int soldado_id = 1;

/* Callback interno de leitura do subsistema Settings */
static int tatico_settings_set(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg)
{
    const char *next;
    
    /* Verifica se a chave que o SO está a carregar é a nossa "soldado_id" */
    if (settings_name_steq(name, "soldado_id", &next) && !next) {
        if (len == sizeof(soldado_id)) {
            /* Lê o valor bruto da memória emulada e atualiza a variável local */
            read_cb(cb_arg, &soldado_id, sizeof(soldado_id));
            LOG_INF("[SETTINGS] ID do Soldado carregado com sucesso da Flash: %d", soldado_id);
            return 0;
        }
    }
    return -ENOENT;
}

/* Estrutura interna que define o grupo "tatico" de configurações */
static struct settings_handler tatico_conf = {
    .name = "tatico",
    .h_set = tatico_settings_set
};

/* IMPLEMENTAÇÃO DAS FUNÇÕES PÚBLICAS */

int tatico_settings_init(void)
{
    /* Regista o nosso handler interno dentro do ecossistema do Zephyr */
    return settings_register(&tatico_conf);
}

int tatico_settings_get_id(void)
{
    return soldado_id;
}

int tatico_settings_save_id(int novo_id)
{
    soldado_id = novo_id;
    /* Guarda o novo valor de forma persistente sob a chave textual "tatico/soldado_id" */
    return settings_save_one("tatico/soldado_id", &soldado_id, sizeof(soldado_id));
}