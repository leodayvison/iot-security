import json
import paho.mqtt.client as mqtt
from paho.mqtt import enums as paho_cte

# constantes universais
KEY = 3
DISPOSITIVOS_VALIDOS = ["dev1", "dev2", "dev3"]

# decripta a cifra de cesar pela tabela ascii
def caesar_decrypt(message, key):
    res = ''
    text = message.payload.decode('utf-8')
    for c in text:
        code = ord(c)
        if 32 <= code <= 126:
            base = 32
            range_ = 95  # 126 - 32 + 1
            code = base + ((code - base - key) % range_)
        res += chr(code)
    return res

# lê a mensagem recebida e transforma em obj
def read_json(s):
    try:
        res = json.loads(s)
        return res
    except Exception as e:
        print("A mensagem recebida não está formatada como JSON", e)

# verifica se o dispositivo ta na lista de dispositivos validos
def device_validator(msg):
    if msg['device_id'] not in DISPOSITIVOS_VALIDOS:
        return False
    return True


# FUNCOES DO MQTT
def on_connect(client, userdata, flags, reason_code, properties=None):
    """"callback pra conexão com o broker bem-sucedida"""

    print(f"Conectado com result code {reason_code}")
    client.subscribe("equipe/sensor")

def on_connect_fail(client, userdata):
    print("Ocorreu um erro na conexão com o broker")


def on_message(client, userdata, msg):
    """"callback pra recepcao de mensagem"""
    print(msg.payload.decode('utf-8'))
    try:
        decoded_message = caesar_decrypt(msg, KEY)
        print("Mensagem decodificada:", decoded_message)
        decoded_dict = read_json(decoded_message) 
        decode_flag = True
        if device_validator(decoded_dict) and decode_flag:
            print(f"string recebida: {decoded_message}") #TODO arrumar pra guardar os dados
        elif not device_validator(decoded_dict): 
            print("ALERTA: Dispositivo não identificado.")
    except Exception as e:
        print("Erro ao processar mensagem:", e)
        decode_flag = False
        decoded_dict = None

    

# instância de cliente mqtt
mqttc = mqtt.Client(paho_cte.CallbackAPIVersion.VERSION2)
mqttc.on_connect = on_connect
mqttc.on_connect_fail = on_connect_fail
mqttc.on_message = on_message

mqttc.connect("test.mosquitto.org", 1883, 60)
mqttc.loop_forever()