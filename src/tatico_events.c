#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "tatico_events.h" /* Inclui o próprio cabeçalho */

/* Registro do módulo de log para este arquivo */
LOG_MODULE_REGISTER(tatico_events, LOG_LEVEL_INF);

/* Criar o ouvinte (Subscriber) que reage às mensagens */
void listener_tatico_cb(const struct zbus_channel *chan)
{
    /* Extrai a mensagem do canal */
    const struct tatico_msg *msg = zbus_chan_const_msg(chan);

    LOG_WRN("ALERTA CAPTURADO PELO ZBUS! >>>SOLDADO %d com Nivel de alerta: %d <<<", msg->origem_id, msg->nivel_alerta);
}

/* Definir o canal do ZBUS e registrar quem são os Listeners */
ZBUS_LISTENER_DEFINE(listener_tatico, listener_tatico_cb);

ZBUS_CHAN_DEFINE(canal_tatico,                   /* Nome do canal */
                 struct tatico_msg,              /* Tipo de dado da mensagem */
                 NULL, NULL,                     /* Validadores (não utilizados) */
                 ZBUS_OBSERVERS(listener_tatico),/* Quem escutará esse canal */
                 ZBUS_MSG_INIT(0));              /* Valor inicial da mensagem */