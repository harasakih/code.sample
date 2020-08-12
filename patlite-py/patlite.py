import socket

class Patlite:
    # default dest
    _host = '10.0.150.74'
    _port = 10000
    _timeout = 5

    OFF = 0
    ON = 1

    _bitidx = {
        'LIGHT_R' : 0,
        'LIGHT_Y' : 1,
        'LIGHT_G' : 2,
        'BUZZ_S'  : 3,
        'BUZZ_L'  : 4,
        'BLINK_R' : 5,
        'BLINK_Y' : 6,
        'BLINK_G' : 7
    }

    _bitkey = [ 
        'LIGHT_R', 'LIGHT_Y', 'LIGHT_G',
        'BUZZ_S', 'BUZZ_L',
        'BLINK_R', 'BLINK_Y', 'BLINK_G' ]


    _sensor = {
        'LIGHT_R' : OFF,
        'LIGHT_Y' : OFF,
        'LIGHT_G' : OFF,
        'BUZZ_S'  : OFF, 
        'BUZZ_L'  : OFF, 
        'BLINK_R' : OFF,
        'BLINK_Y' : OFF,
        'BLINK_G' : OFF
    }


    '''
    シングルトーンパターンで設計
    http://www.denzow.me/entry/2018/01/28/171416
    '''
    _unique_instance = None

    def __new__(self):
        raise NotImplementedError('[err] not permitted')

    # create instance for internal class
    @classmethod
    def __internal_new__(cls):
        return super().__new__(cls)

    @classmethod
    def get_instance(cls):
        if not cls._unique_instance:
            cls._unique_instance = cls.__internal_new__()
        
        return cls._unique_instance


    def set_dest(self, host=_host, port=_port):
        if host is None:
            raise ValueError("[err] host must not empty")

        iport = int(port)
        if port is None or not(0 <= iport <= 65535):
            raise ValueError("[err] port must be set integer")
        
        self._host = host
        self._port = iport
        
        '''
        print("socket to %s:%d" % (host, iport))
        '''

    def set_status(self, name, value):
        self._sensor[name] = value
    
    def get_status(self):
        return self._sensor

    def reset_status(self):
        for k in self._sensor:
            self._sensor[k] = self.OFF

    def commit(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            try:
                s.connect((self._host, self._port))
            except OSError as e:
                print("[err] Cannot connect to patlite. Recheck for address or port.")
                return
            
            icmd = 0x57
            cmd = icmd.to_bytes(1, 'big')  # 'W' バイトオブジェクトとして定義
            idat = 0
            for k in self._sensor:
                idat = idat | (self._sensor[k] << self._bitidx[k] )
            bdat = idat.to_bytes(1,'big')

            print("Send %c %02x" % (icmd, idat))
            s.sendall(cmd + bdat)
            data = s.recv(1024)
            print('Received', repr(data))

