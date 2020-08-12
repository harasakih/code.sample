from bottle import route, request, response, template, run, error
from patlite import Patlite as p
import time

'''
  http://xxxx:_port/patlite?dest=xxx&port=xxx&p=111&timeout=xx
'''

@route('/patlite')
def index():
    if request.query.dest is "":
        dest = p._host
    else:
        dest = request.query.dest

    if request.query.port is "":
        port = p._port
    else:
        port = int(request.query.port)

    '''
    bitstr = b7 - b0
      b7 : BLINK_G
      b0 : LIGHT_R
    '''
    if request.query.p is "":
        bitstr = "00000000"
    else:
        bitstr = request.query.p

    if request.query.timeout is "":
        timeout = p._timeout
    else:
        timeout = int(request.query.timeout)

    msg = 'dest=%s, port=%d, bitstr=%s, timeout=%d' % (dest, port, bitstr, timeout)
    print(msg)

    pat = p.get_instance()
    pat.set_dest(dest, port)

    for i in range(8):
        bit = bitstr[i:i+1]
        if bit is "0":
            bit = p.OFF
        else:
            bit = p.ON

        name = p._bitkey[7-i]
        val = bit
        pat.set_status(name, val)

    pat.commit()

    ## reset時は、即時リターン
    if bitstr == "00000000":
        return 'Patlite Reset(' + msg + ')'

    ## reset以外は、一定時間状態を保持し
    ## ブザーのみオフし、ライトを点灯しておく
    # delay
    time.sleep( int(timeout) )

    # Buzzer off してライトを点灯
    for i in range(8):
        bit = bitstr[i:i+1]
        if p._bitkey[7-i] is "BUZZ_S" or \
           p._bitkey[7-i] is "BUZZ_L" : 
            bit = p.OFF
        elif bit is "0":
            bit = p.OFF
        else:
            bit = p.ON
    
        name = p._bitkey[7-i]
        val = bit
        pat.set_status(name, val)
    
    pat.commit()
    return 'Patlite Success(' + msg + ')'

@error(404)
def error404(error):
    return 'Nothing here, sorry'

@error(500)
def error500(error):
    return 'Internal error, sorry'

run(host='0.0.0.0', port=8080)
