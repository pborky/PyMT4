import unicodedata
import time

def unicode2ascii(unicode_string):
    ascii_string = unicodedata.normalize('NFKD', unicode_string).encode('ascii', 'ignore')
    return ascii_string

def printStartDateTime():
    time_start = time.time()
    now = time.strftime("%c")
    print ('Start Date Time: %s' % now)
    return time_start

def printEndDateTime(time_start):
    time_end = time.time()
    time_delta = time_end - time_start
    hours, hours_remainder = divmod(time_delta, 60 * 60)
    minutes, seconds = divmod(hours_remainder, 60)
    print ('Elapsed (H:m:s) %d:%d:%d' % (hours, minutes, seconds))
    now = time.strftime("%c")
    print ('End Date Time: %s' % now)
    return time_delta