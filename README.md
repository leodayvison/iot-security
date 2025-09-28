# Introdução
Durante o Processo Seletivo do Programa de Educação Tutorial da Engenharia de Computação da Universidade Federal do Ceará, foi-nos proposto o desenvolvimento de um projeto voltado ao tema Cybersegurança.

# Conceito do Projeto
O projeto consiste na simulação de um sistema básico de IoT, em que uma ESP32 recebe um valor de um sensor periodicamente. O foco do projeto está na transmissão dos valores captados via MQTT e na simulação de um protocolo de segurança SSL simplificado, utilizando Cifra de César ao invés de algoritmos como AES128, além de um atributo fixo para o dispositivo ao invés de certificados SSL.

# Tecnologias usadas
- ESP32: Wi-fi nativo, maior poder de processamento e memória, documentação abundante
- MQTT: Protocolo leve, conexão contínua, ideal para IoT
- Python: Sintaxe simples, bibliotecas ricas, documentação abundante, escalabilidade para projetos com dados

# Equipe e divisão de tarefas
- Raul Falcão: Simulação dos sensores e criptografia
- Rodrigo Nogueira: Simulação dos sensores e criptografia
- Maria Paula Mesquita: Publicação de dados para broker MQTT
- Leonardo Dayvison: Descriptografia e validação dos dados recebidos