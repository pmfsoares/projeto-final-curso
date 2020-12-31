#Python script, for sending sound samples to the esp through the serial port


import serial, time, csv, sys, os, json, struct


csv_vals = []
line = 0

class ReadLine:
    def __init__(self, s):
        self.buf = bytearray()
        self.s = s

    def readline(self):
        i = self.buf.find(b"\n")
        if i >= 0:
            r = self.buf[:i+1]
            self.buf = self.buf[i+1:]
            return r
        while True:
            i = max(1, min(2048, self.s.in_waiting))
            data = self.s.read(i)
            i = data.find(b"\n")
            if i >= 0:
                r = self.buf + data[:i+1]
                self.buf[0:] = data[i+1:]
                return r
            else:
                self.buf.extend(data)

def extractCSV(filename):
    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter = ',')
        line_count = 0
        name = ""
        for row in csv_reader:
            if line_count == 0:
                name = row[0]
                line_count += 1
            else:
                mean = 0
                csv_vals.append(row[0])

def serRead():
    if(ser.isOpen()):
        buf = ser.readline()
        data = buf.decode('utf-8')
        print(buf)

def getVal():
    global line
    line = line + 1
    print("Line: " + str(line))
    return float(csv_vals[line - 1])
    
    
ser = serial.Serial("/dev/ttyUSB0", 230400)
rl = ReadLine(ser)

if(ser.isOpen() == False):
    ser.open()

extractCSV("samples.csv")


while True:
    st = rl.readline().decode('utf-8')
    print(st);
    if "val" in st:
        data = struct.pack('d', getVal())
        print(line)
        ser.write(data)
        
#        json_str = json.dumps({'v', -3.14})
        #x = {
        #    "v": 9#.0000000009
        #}
        #y = json.dumps(x);
        #ser.write(y.encode());
        #ser.write(bytes(getVal(), encoding='utf-8'));