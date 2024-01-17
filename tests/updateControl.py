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
from enum import Enum

class ParameterType(Enum): 
  INTEGER = 0
  STRING = 1
  FLOAT = 2
  DOUBLE = 3
  BOOL = 4
  COMMAND= 5
  MENU = 6
  CATEGORY = 7
  REGISTER = 8
  UNKNOWN = 9
  U8_COMPOUND = 10
  U16_COMPOUND = 11
  U32_COMPOUND = 12
  JSON = 13
  SPECIAL_COMPOUND = 14
    

def main():
    if len(sys.argv) == 2:
        print('usage: publisher <bind-to>')
        connection_string = sys.argv[1]
        
    else:
        connection_string="tcp://localhost:4001"

    

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
        'control': {'name': 'ENABLE_EAN13', 'value': True, 'type': int(ParameterType.BOOL.value)}
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

    
