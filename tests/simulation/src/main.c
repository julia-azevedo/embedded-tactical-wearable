#include <zephyr/ztest.h>
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>

#include "tatico_button.h"
#include "tatico_settings.h"
#include "tatico_events.h"

/* Variável global espiã = saber último alerta gerado*/
static int ultimo_alerta_capturado = 0;

/* Listener falso apenas para interceptar os dados no teste*/
void test_spy_cb(const struct zbus_channel *chan)
{
    const struct tatico_msg *msg = zbus_chan_const_msg(chan);
    ultimo_alerta_capturado = msg->nivel_alerta;
}
ZBUS_LISTENER_DEFINE(test_spy, test_spy_cb);

/* Função vai rodar antes de começar a suíte de testes*/
static void *suite_setup(void)
{
    /* Garantir que subsistemas e memórias estejam prontos*/
    settings_subsys_init();
    tatico_settings_init();

    /*Conecta o espião ao canal oficial do sistema*/
    zbus_chan_add_obs(&canal_tatico, &test_spy, K_NO_WAIT);
    return NULL;
}

ZTEST_SUITE(tatico_button_suite, NULL, suite_setup, NULL, NULL, NULL);

/* TESTE 1: CLIQUE SIMPLES*/
ZTEST(tatico_button_suite, test_clique_curto_gera_alerta_1)
{
    ultimo_alerta_capturado = 0;

    tatico_button_update(true); /*aperto do botao*/
    k_msleep(150); /* Segura por 150ms, válido pois > 50ms*/
    tatico_button_update(false); /* solta o botão*/

    k_msleep(500); /*Espera o tempo do duplo clique (400ms) expirar*/

    tatico_button_update(false); /* Para não falhar nos testes, é preciso resetar a máquina de estado */

    /*Validação do ZTEST se a máquina tomou a decisão certa*/
    zassert_equal(ultimo_alerta_capturado, 1, "O clique curto não gerou alerta nível 1 esperado...");
}

/* TESTE 3: CLIQUE DUPLO
ZTEST(tatico_button_suite, test_clique_duplo_gera_alerta_2)
{
    ultimo_alerta_capturado = 0;


}*/

/* TESTE 2: CLIQUE LONGO (SOS)*/
ZTEST(tatico_button_suite, test_clique_longo_gera_alerta_3)
{
    ultimo_alerta_capturado = 0;

    tatico_button_update(true); /*Aperta botão*/
    k_msleep(1600); /*Segura por mais tempo do que 1.5s*/
    tatico_button_update(false); /*Solta o botão*/

    zassert_equal(ultimo_alerta_capturado, 3, "O clique não gerou alerta SOS!");
}