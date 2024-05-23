#!/usr/bin/env python3
from flask import Flask, request

from decode_measures import decode_measures


app = Flask(__name__)
app.config['MAX_CONTENT_LENGTH'] = 16 * 1000 * 1000


@app.route('/measures', methods=['POST'])
def binary_measures():
    print(decode_measures(request.files['bin'].read()))
    return ''


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
