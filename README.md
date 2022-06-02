# OBJETIVOS

Este projeto tem por objetivo piscar o LED integrado na placa STM32F411. Para isso, basta baixar o repositório e aplicar o comando "make" que os arquivos serão compilados e será gerado o .bin necessário para rodar na placa.

# startup.c

- Aloca os vetores de interrupção;
- Chama a função main.

# main.c

- Configura os periféricos necessários;
- Lê o algoritmo de piscar o LED.

# linker.ld

- Cria as seções dos vetores de interrupção, texto e rodata na memória.

# Makefile

- Cria os arquivos .elf, .o e .bin, sendo o último o arquivo que será executado pela placa.