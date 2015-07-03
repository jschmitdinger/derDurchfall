#derDurchfall
Estrutura do código

O código está dividido entre os arquivos:

-Define.h
-Engine.h
-Structs.h

-Main.c
-Engine.c

Para uma melhor estruturação do código, ele foi dividido em funções dentro do arquivo Engine.h.
As funções estão divididas em funções de física, inicialização, ataque, timer, mapa e personagens.
O código da Main, serve apenas para chamar as funções da Engine.h e desenhar os gráficos.

A Main é dividida em estágios:
-jogo
-menu
-pontuações
-pause

A lógica do jogo é feita no estágio "jogo".
No final do código da main, todas as imagens são desenhadas.

No arquivo Engine.h, são declaradas todas as funções da Engine.c.

No arquivo Structs.h são declaradas todas as estruturas do jogo.
O jogo quase não apresenta variáveis independentes. 
Quase todas as variáveis estão ligadas a structs.
Os jogadores e os inimigos são strcuts do tipo AVATAR.
As estruturas apresentam diferentes tipos de variaveis, como vetores, ponteiros e outras estruturas.

O mapa do jogo é lido de um arquivo txt.
O mapa contém as informações dos vitais e das paredes do jogo.
O mapa contém as informações das rotas dos inimigos.

É possível mudar totalmente o mapa do jogo, alterando apenas os valores do txt e os gráficos são redesenhados automaticamente.

O jogo apresenta as bibliotecas:

<stdio.h> 
<stdlib.h>
<math.h>
<string.h>
/////FUNÇÕES DO ALLEGRO/////
<allegro5/allegro.h> 
<allegro5/allegro_primitives.h> 
<allegro5/allegro_image.h> 
<allegro5/allegro_font.h> 
<allegro5/allegro_ttf.h>
<allegro5/allegro_audio.h>
<allegro5/allegro_acodec.h>






