from flask import Flask, request


app = Flask(__name__)


@app.route('/')
def index():
    return 'Nothing here'

measurements = {}

@app.route('/measure/<measurement>', methods=['POST'])
def sensor(measurement: str):
    measurements[measurement] = request.get_json()
    return ''

@app.route('/last/')
def last_list():
    return '/' + '<br>/'.join(measurements.keys())

@app.route('/last/<measurement>')
def last(measurement: str):
    return measurements.get(measurement, '<no data for this measurement yet>')


if __name__ == '__main__':
    app.run('0.0.0.0', 5000)
