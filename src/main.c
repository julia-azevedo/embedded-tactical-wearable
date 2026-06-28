#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>
#include <zephyr/zbus/zbus.h>

/* Importa os nossos módulos recém-criados */
#include "tatico_events.h"
#include "tatico_settings.h"

LOG_MODULE_REGISTER(tatico_main, LOG_LEVEL_INF);

int main(void)
{
    LOG_INF("Iniciando Simulacao do Sistema Tatico Vestivel...");

    /* 1. Inicializa o subsistema base de settings do Zephyr */
    int rc = settings_subsys_init();
    if (rc) {
        LOG_ERR("Falha ao inicializar o Settings Subsys: %d", rc);
        return rc;
    }

    /* 2. Inicializa o nosso módulo de memória específico */
    rc = tatico_settings_init();
    if (rc) {
        LOG_ERR("Falha ao registrar handler do Settings: %d", rc);
        return rc;
    }

    /* 3. Força o Zephyr a ler a flash e carregar os valores */
    rc = settings_load();
    if (rc) {
        LOG_ERR("Erro ao carregar configuracoes da flash: %d", rc);
    }

    /* Variável para montar a mensagem simulada */
    struct tatico_msg msg_mock;

    /* Loop Principal (Onde a nossa máquina de estados vai morar na próxima etapa) */
    while(1)
    {
        /* Pega o ID seguro chamando o getter do módulo de configurações */
        int id_atual = tatico_settings_get_id();

        msg_mock.nivel_alerta = 3; 
        msg_mock.origem_id = id_atual;

        /* Publica essa mensagem no canal do ZBUS (declarado no tatico_events.h) */
        zbus_chan_pub(&canal_tatico, &msg_mock, K_NO_WAIT);

        /* Aguarda 4s para o próximo teste */
        k_msleep(4000); 
    }

    return 0;
}