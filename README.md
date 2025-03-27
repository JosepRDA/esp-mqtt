## Projeto esp32 com conexão MQTT

#### Do que se trata?
- Se trata de um projeto básico que realiza trocas de dados utilizando uma conexão MQTT básico e um broker.

#### Atributos do projeto
- Programado e compilado para a plataforma, tornando-o mais otimizado;
- Utiliza de criptografia(LTS) para proteger os dados do usuário;
- Muito simples, requer apenas um esp32 com conexão MQTT e WiFi.

#### Como utilizar
- Dependências:
    - Compilador C/C++(GCC, Clang, MSVC, etc.);
    - CMake(build);
    - Framework da esp(https://github.com/espressif/esp-idf).
- Como compilar e rodar:
    ``bash
        idf.py build # isso utiliza das ferramentas da esp para compilar o projeto
        idf.py flash -P <porta usb> # transfere o código compilado para a placa
    ``
    - OBS: `idf.py` só estará disponível depois da instalação das ferramentas *esp* no sistema.

