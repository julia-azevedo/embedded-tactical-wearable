#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/shell/shell.h>
#include <stdlib.h>


/* Registro do módulo de log para main.c*/
LOG_MODULE_REGISTER(tatico_main, LOG_LEVEL_INF);

/* Variável global que guarda o ID do soldado atual*/
static int soldado_id = 1;

/*Definir a estrutura da mensagem que irá trafegar no Channel*/
struct tatico_msg {
    int nivel_alerta; /* 1 = "Normal" 2 = "Aviso" 3 = "Crítico SOS" */
    int origem_id; /* Agora a mensagem saberá qual soldado a enviou*/
};

/*Criar o ouvinte (Subscriber) que reage às mensagens*/
void listener_tatico_cb(const struct zbus_channel *chan)
{
    /*Extrai a mensagem do canal*/
    const struct tatico_msg *msg = zbus_chan_const_msg(chan);

    LOG_WRN("ALERTA CAPTURADO PELO ZBUS! >>>SOLDADO %d com Nível de alerta: %d <<<", msg->origem_id, msg->nivel_alerta);
}

/* Definir o canal do ZBUS e registrar quem são os Listeners*/
ZBUS_LISTENER_DEFINE(listener_tatico, listener_tatico_cb);
ZBUS_CHAN_DEFINE(canal_tatico, // Nome do canal
                 struct tatico_msg, // Tipo de dado da mensagem
                 NULL, NULL, // Validadores de envio e recebimento (não utilizados)
                 ZBUS_OBSERVERS(listener_tatico), // Quem escutará esse canal
                 ZBUS_MSG_INIT(0)); // Valor inicial da mensagem

/* LÓGICA DO SHELL*/
/* Função que será executada ao executar o comando no terminal*/
static int cmd_set_id(const struct shell *shell, size_t argc, char **argv)
{
    if (argc < 2) {
        shell_print(shell, "Erro: Informe o ID do soldado. Ex: tatico set_id x");
        return -EINVAL;
    }
    soldado_id = atoi(argv[1]);
    shell_print(shell, "Sucesso! ID do dispositivo alterado para %d", soldado_id);

    return 0;
}

/* Registro do comando no SO. 
   Hierarquia: tatico -> set_id -> [funcao]
*/
SHELL_STATIC_SUBCMD_SET_CREATE(sub_tatico,
    SHELL_CMD(set_id, NULL, "Configura o ID do soldado. Ex: tatico set_id x", cmd_set_id),
    SHELL_SUBCMD_SET_END /* Terminador obrigatório!!*/
);
SHELL_CMD_REGISTER(tatico, &sub_tatico, "Comando do dispositivo tático", NULL);



int main(void)
{
    LOG_INF("Iniciando Simulação do Sistema Tático Vestível..."); // INF = informação do início da simulaçã

    struct tatico_msg msg_mock;

    while(1)
    {
        /*Simula o soldado apertando e segurando o botão.*/
        msg_mock.nivel_alerta = 3; 
        msg_mock.origem_id = soldado_id; /* Pega o ID configurado no SHELL*/

        /*Publica essa mensagem no canal do ZBUS*/
        zbus_chan_pub(&canal_tatico, &msg_mock, K_NO_WAIT);

        /*Aguarda 4s para o próximo teste*/
        k_msleep(4000); 
    }

    return 0;
}