from flask import Flask, request


app = Flask(__name__)


@app.route('/')
def index():
    return 'Nothing here'

@app.route('/measure/<measurement>', methods=['POST'])
def sensor(measurement: str):
    print(request.get_json())
    return ''


if __name__ == '__main__':
    app.run('0.0.0.0', 5000)
