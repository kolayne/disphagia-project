from flask import Flask, request


app = Flask(__name__)


@app.route('/')
def index():
    return 'Nothing here'

measurements = {}

@app.route('/measure/<sensing_device>', methods=['POST'])
def sensor(sensing_device: str):
    measurements[sensing_device] = request.get_json()
    return ''

@app.route('/last/')
def last_list():
    return '/' + '<br>/'.join(measurements.keys())

@app.route('/last/<sensing_device>')
def last(sensing_device: str):
    return measurements.get(sensing_device, '<no data for this measurement yet>')


if __name__ == '__main__':
    app.run('0.0.0.0', 5000)
