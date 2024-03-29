
#! /usr/bin/python2

import time
import sys
from datetime import datetime
import getopt
import RPi.GPIO as GPIO
from hx711 import HX711

#https://github.com/tatobari/hx711py
#https://github.com/j-dohnalek/hx711py
referenceUnit =3250
offset=379450
def cleanAndExit():
    #print("Cleaning...")

    GPIO.cleanup()
        
    #print("Bye!")
    sys.exit()


def write2file(arnia,val):
    # Opening a file
    file1 = open('/home/pi/workspace/MyBeeMonitor/python/weight.txt', 'w')
    now = datetime.now()
    # Writing a string to file
    file1.write(now.strftime("%d/%m/%Y %H:%M:%S") + ";" + str(arnia) + ";" + str(val) + ";\n")
    
    # Closing file
    file1.close()

def main(argv):
    GPIO.setwarnings(False)
    dout = 5
    sck = 6
    arnia = 0
    opts, args = getopt.getopt(argv,"ha:d:s:",["arnia=","dout=","sck="])
    for opt, arg in opts:
        if opt == '-h':
            print ('myhx711py.py -a <arnia> -d <dout> -s <sck>')
            sys.exit()
        elif opt in ("-a", "--arnia",):
            arnia = arg
        elif opt in ("-d", "--dout"):
            dout = arg
        elif opt in ("-s","--sck"):
            sck = arg

    hx = HX711(int(dout), int(sck))

    # I've found out that, for some reason, the order of the bytes is not always the same between versions of python, numpy and the hx711 itself.
    # Still need to figure out why does it change.
    # If you're experiencing super random values, change these values to MSB or LSB until to get more stable values.
    # There is some code below to debug and log the order of the bits and the bytes.
    # The first parameter is the order in which the bytes are used to build the "long" value.
    # The second paramter is the order of the bits inside each byte.
    # According to the HX711 Datasheet, the second parameter is MSB so you shouldn't need to modify it.
    hx.set_reading_format("MSB", "MSB")

    # HOW TO CALCULATE THE REFFERENCE UNIT
    # To set the reference unit to 1. Put 1kg on your sensor or anything you have and know exactly how much it weights.
    # In this case, 92 is 1 gram because, with 1 as a reference unit I got numbers near 0 without any weight
    # and I got numbers around 184000 when I added 2kg. So, according to the rule of thirds:
    # If 2000 grams is 184000 then 1000 grams is 184000 / 2000 = 92.
    #hx.set_reference_unit(113)
    hx.set_reference_unit(referenceUnit)

    hx.reset()

    #hx.tare()
    tar = hx.read_average()
    print(tar)
    hx.set_offset(offset)
    #print("Tare done! Add weight now...")

    # to use both channels, you'll need to tare them both
    #hx.tare_A()
    #hx.tare_B()


    try:
        # These three lines are usefull to debug wether to use MSB or LSB in the reading formats
        # for the first parameter of "hx.set_reading_format("LSB", "MSB")".
        # Comment the two lines "val = hx.get_weight(5)" and "print val" and uncomment these three lines to see what it prints.
        
        # np_arr8_string = hx.get_np_arr8_string()
        # binary_string = hx.get_binary_string()
        # print binary_string + " " + np_arr8_string
        
        # Prints the weight. Comment if you're debbuging the MSB and LSB issue.
        val = hx.get_weight(5)
        print(val)
        write2file(arnia,val)

        # To get weight from both channels (if you have load cells hooked up 
        # to both channel A and B), do something like this
        #val_A = hx.get_weight_A(5)
        #val_B = hx.get_weight_B(5)
        #print "A: %s  B: %s" % ( val_A, val_B )

        #hx.power_down()
        #hx.power_up()
        time.sleep(0.1)

    except (KeyboardInterrupt, SystemExit):
        cleanAndExit()

    cleanAndExit()

if __name__ == "__main__":
   main(sys.argv[1:])
