from flask import Flask, jsonify
from flask_cors import CORS
import serial
import time

app = Flask(__name__)
CORS(app)

PORT_BLUETOOTH = 'COM7' 
BAUD_RATE = 9600

try:
    print(f"Incercare conectare la {PORT_BLUETOOTH}...")
    bt = serial.Serial(PORT_BLUETOOTH, BAUD_RATE, timeout=1)
    time.sleep(2)
    print("Succes!")
except Exception as e:
    print(f"Eroare la conectare: {e}")
    bt = None

@app.route('/lampa/<comanda>', methods=['POST'])
def control_lampa(comanda):
    if not bt:
        return jsonify({"status": "eroare", "mesaj": "Placa nu este conectata"}), 500
    
    try:
        if comanda == 'on':
            bt.write(b'1')
            bt.flush()
        elif comanda == 'off':
            bt.write(b'0')
            bt.flush()
        elif comanda == 'auto':
            bt.write(b'A')
            bt.flush()
        else:
            return jsonify({"status": "eroare", "mesaj": "Comanda invalida"}), 400
            
        return jsonify({"status": "succes", "comanda": comanda}), 200
    except Exception as e:
        return jsonify({"status": "eroare", "mesaj": str(e)}), 500

if __name__ == '__main__':
    print("Serverul asculta pe http://localhost:5000")
    app.run(host='0.0.0.0', port=5000)