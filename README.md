## Projeto esp32 com conexão MQTT

#### Do que se trata?
- Se trata de um projeto básico que realiza trocas de dados utilizando uma conexão MQTT básico e um broker.

#### Atributos do projeto
- Programado e compilado para a plataforma, tornando-o mais otimizado;
- Muito simples, requer apenas um esp32 com conexão MQTT e WiFi.

#### Como utilizar
- Dependências:
    - Compilador C/C++(GCC, Clang, MSVC, etc.);
    - CMake(build);
    - Framework da esp(https://github.com/espressif/esp-idf).

#### Compilação e Flash
```bash
# Compila o projeto usando as ferramentas do ESP-IDF
idf.py build

# Grava o firmware na placa (substitua <porta_usb> pela sua porta, ex: /dev/ttyUSB0 ou COM3)
idf.py flash -p <porta_usb>
```
