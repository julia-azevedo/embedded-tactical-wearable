#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "tatico_button.h"
#include "tatico_events.h"
#include "tatico_settings.h"

LOG_MODULE_REGISTER(tatico_button, LOG_LEVEL_INF);

/* Definição dos Estados possíveis */
typedef enum {
    STATE_IDLE,
    STATE_PRESSED,
    STATE_WAIT_DOUBLE,
    STATE_WAIT_RELEASE
} button_state_t;

/* Variáveis de controle da máquina de estados */
static button_state_t current_state = STATE_IDLE;
static uint32_t state_time = 0;

/* Função auxiliar para publicar no Zbus de forma limpa */
static void disparar_alerta(int nivel)
{
    struct tatico_msg msg;
    msg.nivel_alerta = nivel;
    msg.origem_id = tatico_settings_get_id(); /* Pega o ID salvo na flash */
    
    zbus_chan_pub(&canal_tatico, &msg, K_NO_WAIT);
}

void tatico_button_update(bool is_pressed)
{
    uint32_t now = k_uptime_get_32(); /* Tempo atual do sistema em milissegundos */

    switch (current_state) {
        
        case STATE_IDLE:
            if (is_pressed) {
                current_state = STATE_PRESSED;
                state_time = now; /* Marca a hora exata que apertou */
            }
            break;

        case STATE_PRESSED:
            if (!is_pressed) {
                /* Soltou o botão! Vamos calcular quanto tempo durou */
                uint32_t press_duration = now - state_time;
                
                if (press_duration < 50) {
                    /* DEBOUNCE: Foi rápido demais, menos de 50ms. É ruído elétrico! Ignora. */
                    current_state = STATE_IDLE;
                } 
                else if (press_duration > 1500) {
                    /* CLIQUE LONGO: Mais de 1.5 segundos. É um SOS! */
                    LOG_ERR(">> CLIQUE LONGO DETECTADO! Disparando SOS (Nivel 3) <<");
                    disparar_alerta(3);
                    current_state = STATE_IDLE;
                } 
                else {
                    /* CLIQUE CURTO: Foi um clique normal. 
                       Vamos para o estado de espera para ver se vem um segundo clique! */
                    current_state = STATE_WAIT_DOUBLE;
                    state_time = now; /* Zera o relógio para a espera */
                }
            }
            break;

        case STATE_WAIT_DOUBLE:
            if (is_pressed) {
                /* O soldado apertou de novo antes do tempo acabar! */
                LOG_WRN(">> DUPLO CLIQUE DETECTADO! Disparando Aviso (Nivel 2) <<");
                disparar_alerta(2);
                current_state = STATE_WAIT_RELEASE; /* Trava até ele soltar o dedo */
            } 
            else if ((now - state_time) > 400) {
                /* Passaram-se 400ms e ele não apertou de novo. Era só um clique simples. */
                LOG_INF(">> CLIQUE SIMPLES DETECTADO! Status Normal (Nivel 1) <<");
                disparar_alerta(1);
                current_state = STATE_IDLE;
            }
            break;

        case STATE_WAIT_RELEASE:
            /* Apenas espera o botão voltar para o nível baixo (soltar) para recomeçar */
            if (!is_pressed) {
                current_state = STATE_IDLE;
            }
            break;
    }
}