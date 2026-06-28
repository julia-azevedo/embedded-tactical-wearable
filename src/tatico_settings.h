#ifndef TATICO_SETTINGS_H
#define TATICO_SETTINGS_H

/* Inicializa o módulo de configurações e regista o handler no Zephyr */
int tatico_settings_init(void);

/* Função pública para obter o ID do soldado atual (Getter) */
int tatico_settings_get_id(void);

/* Função pública para atualizar e salvar o ID do soldado na flash (Setter) */
int tatico_settings_save_id(int novo_id);

#endif /* TATICO_SETTINGS_H */