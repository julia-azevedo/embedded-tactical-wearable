#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/settings/settings.h>

/* Importa as interfaces dos nossos módulos */
#include "tatico_events.h"
#include "tatico_settings.h"
#include "tatico_button.h"

LOG_MODULE_REGISTER(tatico_main, LOG_LEVEL_INF);

int main(void)
{
    LOG_INF("Iniciando Simulacao do Sistema Tatico Vestivel...");

    /* 1. Inicializa o motor principal do settings no Zephyr */
    settings_subsys_init();
    
    /* 2. Inicializa o NOSSO módulo (que mora lá no tatico_settings.c) */
    tatico_settings_init();
    
    /* 3. Força a leitura da Flash */
    settings_load();

    uint32_t tempo_simulacao = 0;
    bool sinal_botao = false;

    /* Loop principal: Roda a cada 10 milissegundos infinitamente */
    while(1)
    {
        /* --- SCRIPT DE SIMULAÇÃO DE CLIQUES --- */
        
        /* Entre 2s e 2.1s -> Aperta e solta rápido (Clique Curto / Nivel 1) */
        if (tempo_simulacao == 2000) { sinal_botao = true; LOG_DBG("Simulando: Dedo apertou"); }
        if (tempo_simulacao == 2100) { sinal_botao = false; LOG_DBG("Simulando: Dedo soltou"); }

        /* Entre 5s e 5.1s -> Aperta. Entre 5.2s e 5.3s -> Aperta (Clique Duplo / Nivel 2) */
        if (tempo_simulacao == 5000) { sinal_botao = true; }
        if (tempo_simulacao == 5100) { sinal_botao = false; }
        if (tempo_simulacao == 5200) { sinal_botao = true; }
        if (tempo_simulacao == 5300) { sinal_botao = false; }

        /* Entre 9s e 11s -> Segura apertado por 2 segundos (Clique Longo SOS / Nivel 3) */
        if (tempo_simulacao == 9000) { sinal_botao = true; }
        if (tempo_simulacao == 11000) { sinal_botao = false; }

        /* --- FIM DA SIMULAÇÃO --- */

        /* Alimenta o "Cérebro" com o estado atual do botão */
        tatico_button_update(sinal_botao);

        /* Dorme curtinho para não travar a máquina de estados (Polling rate de 100Hz) */
        k_msleep(10); 
        tempo_simulacao += 10;
        
        /* Reinicia o roteiro da simulação a cada 15 segundos */
        if (tempo_simulacao >= 15000) tempo_simulacao = 0;
    }
    
    return 0;
}