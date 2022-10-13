# Lilygo T-Watch-2020-v1 
##### Configurações iniciais e display customizado.

## Passo 1:
Baixe e instale o Arduino IDE.
https://www.arduino.cc/en/software

## Passo 2:
Abra o Arduino IDE e acesse as preferências.

![](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/imagens/passo-2.png)

## Passo 3:
Insira o url abaixo em **Additional boards manager URLs**.
`https://dl.espressif.com/dl/package_esp32_index.json`

Em seguida, clique em ok.<br>

![](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/imagens/passo-3.png)

## Passo 4:
Acesse **Tools > Board > Boards Manager...**.

![](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/imagens/passo-4.png)

## Passo 5:
Procure por ESP32 e pressione o botão de instalar em **ESP32 by Espressif Systems**.

![](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/imagens/passo-5.png)

## Passo 6:
Acesse **Tools > Board menu** e selecione **TTGO T-Watch**.

![](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/imagens/passo-6.png)

## Passo 7:
Acesse https://github.com/Xinyuan-LilyGO/TTGO_TWatch_Library e baixe a biblioteca.

![](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/imagens/passo-7.png)

## Passo 8:
Acesse **Sketch > Include Library > Add ZIP Library** e selecione o .zip baixado no passo anterior.

![](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/imagens/passo-8.png)

## Passo 9:
Baixe o zip a seguir e extraia ele. 
https://codeload.github.com/guidiasz/Lilygo-T-Watch-2020-v1/zip/refs/heads/main

## Passo 10:
 Acesse **File > Open**. Na que você acabou de extrair, selecione o arquivo **TWatch_framework_0.ino** e clique em abrir.
 
 ![](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/imagens/passo-10.png)
 
## Passo 11:
 Com o relógio conectado ao computador, acesse **Select Board** e selecione a porta do relógio. No campo **Boards**, selecione TTGO T-WATCH. Em seguida,  clique em **OK**.
 
 ![](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/imagens/passo-11.png)
 
## Passo 12:
 Com o relógio conectado ao computador, acesse **Select Board** e selecione a porta do relógio. No campo **Boards**, selecione TTGO T-WATCH. Em seguida,  clique em **OK**.
 
 ![](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/imagens/passo-12.png)
 
#### Observação:
 Pode ser necessária a instalação do driver do relógio para o funcionamento do passo 11: https://t-watch-document-en.readthedocs.io/en/latest/download/#driver-download
 
## Resultado esperado:

![](https://raw.githubusercontent.com/guidiasz/Lilygo-T-Watch-2020-v1/main/imagens/resultado-esperado.jpg)

## Referências:
- https://www.instructables.com/Lilygo-T-Watch-2020-Arduino-Framework/ ;
- Manual do produto.

## Adcionando apps no relogio:
https://github.com/guidiasz/Lilygo-T-Watch-2020-v1/blob/main/novo-app.md


