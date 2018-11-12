/*
 * lcd.c
 *
 *  Created on: Aug 20, 2018
 *      Author: Renan Augusto Starke
 *      
 *      Adaptado de AVR e Arduino: T�cnicas de Projeto, 2a ed. - 2012.
 *      Instituto Federal de Santa Catarina
 */

#include "lcd.h"

/* sinal de habilita��o para o LCD */
#define PULSO_ENABLE()           \
    _delay_us(1);                \
    SET_BIT(CONTR_LCD->PORT, E); \
    _delay_us(1);                \
    CLR_BIT(CONTR_LCD->PORT, E); \
    _delay_us(45)

/* Stream para utiliza��o do fprintf */
FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

/**
  * @brief  Retorna o stream interno para utilizar a libc (printf no LCD)
  * @param	Nenhum
  *
  * @retval FILE: ponteiro do stream para utiliza com fprintf.
  */
FILE *inic_stream()
{
    return &lcd_str;
}

/**
  * @brief  Envia um dado est�tico para o display: caractere ou comando.
  * @param c: valor do comando.
  * @param cd: 0 para comando. 1 para caractere.
  *
  * @retval Nenhum
  */
void cmd_LCD(uint8_t c, uint8_t cd) //c � o dado  e cd indica se � instru��o ou caractere
{
    if (cd == 0)
        CLR_BIT(CONTR_LCD->PORT, RS);
    else
        SET_BIT(CONTR_LCD->PORT, RS);

//primeiro nibble de dados - 4 MSB
//compila c�digo para os pinos de dados do LCD nos 4 MSB do PORT
#if (NIBBLE_DADOS)
    DADOS_LCD->PORT = (DADOS_LCD->PORT & 0x0F) | (0xF0 & c);
#else
    //compila c�digo para os pinos de dados do LCD nos 4 LSB do POR
    DADOS_LCD->PORT = (DADOS_LCD->PORT & 0xF0) | (c >> 4);
#endif

    PULSO_ENABLE();

//segundo nibble de dados - 4 LSB
#if (NIBBLE_DADOS)
    //compila c�digo para os pinos de dados do LCD nos 4 MSB do PORT
    DADOS_LCD->PORT = (DADOS_LCD->PORT & 0x0F) | (0xF0 & (c << 4));
#else
    //compila c�digo para os pinos de dados do LCD nos 4 LSB do PORT
    DADOS_LCD->PORT = (DADOS_LCD->PORT & 0xF0) | (0x0F & c);
#endif

    PULSO_ENABLE();

    //se for instru��o de retorno ou limpeza espera LCD estar pronto
    if ((cd == 0) && (c < 4))
        _delay_ms(2);
}

/**
  * @brief  Configura hardware: verificar lcd.h para mapa de pinos e nible de dados.
  * @param	Nenhum
  *
  * @retval Nenhum.
  */
void inic_LCD_4bits()
{
    //sequ�ncia ditada pelo fabricando do circuito integrado HD44780
    //o LCD ser� s� escrito. Ent�o, R/W � sempre zero.

    /* Configura pinos de controle */
    SET_BIT(CONTR_LCD->DDR, RS);
    SET_BIT(CONTR_LCD->DDR, E);

/* Configure pinos de dados */
#if (NIBBLE_DADOS)
    DADOS_LCD->DDR |= 0xF0;
#else
    DADOS_LCD->DDR |= 0x0F;
#endif

    //RS em zero indicando que o dado para o LCD ser� uma instru��o
    CLR_BIT(CONTR_LCD->PORT, RS);
    //pino de habilita��o em zero
    CLR_BIT(CONTR_LCD->PORT, E);

    //tempo para estabilizar a tens�o do LCD, ap�s VCC ultrapassar 4.5 V (na pr�tica pode
    //ser maior).
    _delay_ms(20);

//interface de 8 bits
#if (NIBBLE_DADOS)
    DADOS_LCD->PORT = (DADOS_LCD->PORT & 0x0F) | 0x30;
#else
    DADOS_LCD->PORT = (DADOS_LCD->PORT & 0xF0) | 0x03;
#endif

    //habilita��o respeitando os tempos de resposta do LCD
    PULSO_ENABLE();
    _delay_ms(5);
    PULSO_ENABLE();
    _delay_us(200);
    PULSO_ENABLE();

/* At� aqui ainda � uma interface de 8 bits.
    * Muitos programadores desprezam os comandos acima, respeitando apenas o tempo de
    * estabiliza��o da tens�o (geralmente funciona). Se o LCD n�o for inicializado primeiro no 
    * modo de 8 bits, haver� problemas se o microcontrolador for inicializado e o display j� o tiver sido.*/

//interface de 4 bits, deve ser enviado duas vezes (a outra est� abaixo)
#if (NIBBLE_DADOS)
    DADOS_LCD->PORT = (DADOS_LCD->PORT & 0x0F) | 0x20;
#else
    DADOS_LCD->PORT = (DADOS_LCD->PORT & 0xF0) | 0x02;
#endif

    PULSO_ENABLE();
    //interface de 4 bits 2 linhas (aqui se habilita as 2 linhas)
    cmd_LCD(0x28, 0);

    //s�o enviados os 2 nibbles (0x2 e 0x8)

    //desliga o display
    cmd_LCD(0x08, 0);
    //limpa todo o display
    cmd_LCD(0x01, 0);
    //mensagem aparente cursor inativo n�o piscando
    cmd_LCD(0x0C, 0);
    //inicializa cursor na primeira posi��o a esquerda - 1a linha
    cmd_LCD(0x80, 0);
}

/**
  * @brief  Escreve um string est�tica (sem printf) no LCD. 
  * @param c: ponteiro para a string em RAM
  *
  * @retval Nenhum
  */
void escreve_LCD(char *c)
{
    for (; *c != 0; c++)
        cmd_LCD(*c, 1);
}

/**
  * @brief  Escreve um string est�tica (sem printf) no LCD. 
  * @param c: ponteiro para a string em FLASH
  *
  * @retval Nenhum
  */
void escreve_LCD_Flash(const char *c)
{
    for (; pgm_read_byte(&(*c)) != 0; c++)
        cmd_LCD(pgm_read_byte(&(*c)), 1);
}

/**
  * @brief Escreve um caractere no stream. Fun��o utilizada pela libc.
  * @param c: caractere a ser escrito.
  * @param fp: ponteiro do stream inicilizado que receber� o dado.
  *
  * @retval int maior que zero se sucesso.
  */
int lcd_putchar(char c, FILE *fp)
{
    cmd_LCD(c, 1);

    return 1;
}
