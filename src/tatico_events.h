#ifndef TATICO_EVENTS_H
#define TATICO_EVENTS_H

#include <zephyr/zbus/zbus.h>

/* Definir a estrutura da mensagem que irá trafegar no Channel */
struct tatico_msg {
    int nivel_alerta; /* 1 = "Normal", 2 = "Aviso", 3 = "Crítico SOS" */
    int origem_id;    /* Agora a mensagem saberá qual soldado a enviou */
};

/* DECLARA (mas não cria) o canal para que outros arquivos possam publicá-lo */
ZBUS_CHAN_DECLARE(canal_tatico);

#endif /* TATICO_EVENTS_H */