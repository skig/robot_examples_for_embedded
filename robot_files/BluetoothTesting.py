import pygatt.backends

class BluetoothTesting:
 
    ROBOT_LIBRARY_SCOPE = 'GLOBAL'
    
    def read_char_value(self):
        adapter = pygatt.backends.GATTToolBackend()
        try:
            adapter.start()
            device = adapter.connect('f0:08:d1:d5:0c:ae')
            value = device.char_read("ece27bad-3d4b-4072-8494-76a551f0b6cc")
            print(value)
            return value
        finally:
            adapter.stop()