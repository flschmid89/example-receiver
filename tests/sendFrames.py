"""A test that subscribes to NumPy arrays.
Uses REQ/REP (on PUB/SUB socket + 1) to synchronize
"""

# -----------------------------------------------------------------------------
#  Copyright (c) 2010 Brian Granger
#
#  Distributed under the terms of the New BSD License.  The full license is in
#  the file COPYING.BSD, distributed as part of this software.
# -----------------------------------------------------------------------------

import itertools
import sys
import time
import numpy as np
import zmq
import json
import msgpack
import cv2
import random
import os
array = np.ones((1, 4000,3000,3),dtype=np.ubyte)

images = []


for image in os.listdir("/home/notavis/barcode-app/tests/images"):
    image = cv2.imread("/home/notavis/barcode-app/tests/images/"+image,cv2.IMREAD_GRAYSCALE)
    images.append(image)

def main():
    if len(sys.argv) == 2:
        print('usage: publisher <bind-to>')
        connection_string = sys.argv[1]
        
    else:
        connection_string="tcp://localhost:4001"

    
    # image = cv2.imread("/home/notavis/barcode-app/tests/images/2d-barcodes.jpg",cv2.IMREAD_GRAYSCALE)
    # image = cv2.cvtColor(image, cv2.COLOR_GRAY2RGB)

    ctx = zmq.Context()
    s = ctx.socket(zmq.REQ)
    s.connect(connection_string)

    print("Starting broadcast on topics:")
    print("Hit Ctrl-C to stop broadcasting.")
    print("Waiting so subscriber sockets can connect...")
    print("")
    time.sleep(1.0)
    
    for image in images:
    
        width = image.shape[1]
        height = image.shape[0]
        channels = 1 if len(image.shape) == 2 else image.shape[2]
        meta = {
            'dataformat': {
            'rows': height,
            'cols': width,
            'channels': channels,
            'dtype': str(image.dtype)
            }
        }

        setValueMeta = {
            'command':'set_value',
            'control': {'name': 'threshold', 'value': 0, 'type': 0}
        }
        bMeta = msgpack.dumps(meta)

        try:
            counter = 0           
                
                
            print('   Topic: IMAGE , msg:%s' % ( json.dumps(meta['dataformat']) ))

            meta["data"] = image.tobytes()
            print(len (meta["data"]))

            if(counter == 0):
                setValueMeta["control"]["value"] = 255 if setValueMeta["control"]["value"] == 0 else 0
                bMeta = msgpack.dumps(setValueMeta)
            else:
                bMeta = msgpack.dumps(meta)
            s.send( bMeta)

            # image = np.roll(image, -50, axis=1)

            result = s.recv()
            jsonResult = msgpack.unpackb(result, raw=False)
            if("data" in jsonResult):
                input = np.frombuffer(bytes(jsonResult["data"]), np.uint8)
                                                                # input = input * (2^6)
                print(jsonResult["dataformat"])
                rawImage = np.reshape(input,(jsonResult["dataformat"]["rows"], jsonResult["dataformat"]["cols"],jsonResult["dataformat"]["channels"]))

                cv2.imwrite("result.png",rawImage)

            counter += 1

            if(counter == 10):
                counter = 0

                # short wait so we don't hog the cpu
                # time.sleep(0.4)
        except KeyboardInterrupt:
            pass

    print("Waiting for message queues to flush...")
    time.sleep(0.5)
    print("Done.")



if __name__ == "__main__":
    main()

    
