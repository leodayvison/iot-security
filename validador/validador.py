# variaveis universais
CHAVE = 1
DISPOSITIVOS_VALIDOS = ["dev1", "dev2", "dev3"]

# decripta a cifra de cesar pela tabela ascii
def caesar_decrypt(message, key):
    res = ''
    for c in message:
        aux = ord(c) - key
        c = chr(aux)
        res += c
    return res
