import json
import paho.mqtt.client as mqtt

# constantes universais
KEY = 1
DISPOSITIVOS_VALIDOS = ["dev1", "dev2", "dev3"]

# decripta a cifra de cesar pela tabela ascii
def caesar_decrypt(message, key):
    res = ''
    for c in message:
        aux = ord(c) - key
        c = chr(aux)
        res += c
    return res

# lê a mensagem recebida e transforma em obj
def read_json(s):
    res = json.loads(s)
    return res

# verifica se o dispositivo ta na lista de dispositivos validos
def device_validator(msg):
    if msg['device_id'] not in DISPOSITIVOS_VALIDOS:
        return False
    return True


# FUNCOES DO MQTT
def on_connect(client, userdata, flags, reason_code, properties):
    """"callback pra conexão com o broker bem-sucedida"""

    print(f"Conectado com result code {reason_code}")
    client.subscribe("$SYS/#")

def on_message(client, userdata, msg):
    """"callback pra recepcao de mensagem"""
    
    try:
        decoded_message = caesar_decrypt(msg, KEY)
        decoded_dict = read_json(decoded_message)
    except Exception as e:
        print("Não foi possível descriptografar:", e)

    if device_validator(decoded_dict):
        pass #TODO guardar os dados
    else: 
        print("Dispositivo não identificado.")