from flask import Flask
import subprocess

app = Flask(__name__)

@app.route('/shutdown')
def shutdown():
    subprocess.run(['sudo', 'shutdown', 'now'])
    return 'ok'

if __name__ == '__main__':
    app.run(port=5000)
