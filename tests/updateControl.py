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


def main():
    if len(sys.argv) == 2:
        print('usage: publisher <bind-to>')
        connection_string = sys.argv[1]
        
    else:
        connection_string="ipc:///tmp/test.0"

    

    ctx = zmq.Context()
    s = ctx.socket(zmq.REQ)
    s.connect(connection_string)

    print("Starting broadcast on topics:")
    print("Hit Ctrl-C to stop broadcasting.")
    print("Waiting so subscriber sockets can connect...")
    print("")
    time.sleep(1.0)
    


    meta = {
        'command':'set_value',
        'control': {'name': 'threshold', 'value': 33, 'type': 0}
    }
    bMeta = msgpack.dumps(meta)

    try:
        
            
            print('   Topic: IMAGE , msg:%s' % ( bMeta))
            s.send( bMeta)

            result = s.recv()
            jsonResult = msgpack.unpackb(result, raw=False)

            print(jsonResult)

            # short wait so we don't hog the cpu
            time.sleep(0.4)
    except KeyboardInterrupt:
        pass

    print("Waiting for message queues to flush...")
    time.sleep(0.5)
    print("Done.")



if __name__ == "__main__":
    main()

    
