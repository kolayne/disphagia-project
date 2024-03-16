from io import BytesIO
from datetime import datetime
from typing import Optional

from fastapi import FastAPI, Request, HTTPException
from fastapi.responses import StreamingResponse

from dict_stream import flatten_dict, extend_dicts_keys


app = FastAPI()


@app.get('/')
def index():
    return 'Nothing here'


measurements = {}


@app.post('/measures')
async def post_measures(device: str, request: Request):
    measurements[device] = measurements.get(device, []) + [await request.json()]
    return {}

@app.delete('/measures')
async def delete_measures(device: str):
    if device not in measurements.keys():
        raise HTTPException(status_code=404, detail='Unknown device, nothing to delete')
    del measurements[device]
    return {}

@app.get('/last')
async def get_last(device: Optional[str] = None):
    if device is None:
        return {'keys': list(measurements.keys())}
    return measurements.get(device, [{'error': '<no data for this device yet>'}])[-1]

@app.get('/export')
async def get_export(device: str):
    sep = ';'

    if not measurements.get(device):
        raise HTTPException(status_code=404, detail="No measurements for the requested device")

    flattened = map(lambda m: dict(flatten_dict(m)), measurements[device])
    extended = list(extend_dicts_keys(flattened, placeholder_value=''))

    export = ''

    keys = sorted(next(iter(extended)).keys())
    export += sep.join(keys) + '\n'
    for measurement in extended:
        export += sep.join(str(measurement[k]) for k in keys) + '\n'

    csvbin = BytesIO()
    csvbin.write(export.encode('utf-8'))
    csvbin.seek(0)
    device = device.replace('"', '')  # Avoid invalid response below
    return StreamingResponse(csvbin, media_type='text/csv',
                             headers={'Content-Disposition':
                                 f'inline; filename="{device} {datetime.now():%Y-%m-%d %H-%M-%S}.csv"'})


if __name__ == '__main__':
    print("The suggested run method is: uvicorn main:app --host 0.0.0.0")
    exit(1)
