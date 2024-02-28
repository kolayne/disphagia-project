from io import BytesIO
from datetime import datetime

from flask import Flask, request, abort, send_file

from dict_stream import flatten_dict, extend_dicts_keys


app = Flask(__name__)


@app.route('/')
def index():
    return 'Nothing here'


measurements = {}


@app.route('/measures/<sensing_device>', methods=['POST', 'DELETE'])
def sensor(sensing_device: str):
    if request.method == 'DELETE':
        if sensing_device in measurements.keys():
            del measurements[sensing_device]
        else:
            abort(404, 'Unknown sensing device name, nothing to delete')
    elif request.method == 'POST':
        # Add the value to previous measurements
        measurements[sensing_device] = measurements.get(sensing_device, []) + [request.get_json()]

    return ''

@app.route('/last/')
def last_list():
    return '/' + '<br>/'.join(measurements.keys())

@app.route('/last/<sensing_device>')
def last(sensing_device: str):
    return measurements.get(sensing_device, ['<no data for this measurement yet>'])[-1]

@app.route('/export/<sensing_device>')
def export(sensing_device: str):
    sep = ';'

    if not measurements.get(sensing_device):
        abort(404, 'No measurements present for the requested sensing device')

    flattened = map(lambda m: dict(flatten_dict(m)), measurements[sensing_device])
    extended = list(extend_dicts_keys(flattened, placeholder_value=''))

    export = ''

    keys = sorted(next(iter(extended)).keys())
    export += sep.join(keys) + '\n'
    for measurement in extended:
        export += sep.join(str(measurement[k]) for k in keys) + '\n'

    csvbin = BytesIO()
    csvbin.write(export.encode('utf-8'))
    csvbin.seek(0)
    return send_file(csvbin, mimetype='text/csv',
                     download_name=f'{sensing_device} {datetime.now():%Y-%m-%d %H-%M-%S}.csv')


if __name__ == '__main__':
    app.run('0.0.0.0', 5000)
