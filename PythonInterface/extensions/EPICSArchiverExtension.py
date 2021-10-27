import urllib.request as urllib2
import json
from datetime import datetime, date, time, timedelta

class EPICSArchiver(object):

    def __init__(self):
        self.archiver_root = "http://claraserv2.dl.ac.uk:17668/retrieval/data/"
    def _printMessage(self, message):
        print(message)
    
    def _formURLStr(self, pv, dateFrom, timeFrom, dateTo, timeTo):
        pv = pv.replace(':', '%3A')
        urlString = ("getData.json?pv=" + str(pv) +
                     "&from=" + str(dateFrom.year) +
                     "-" + str(dateFrom.month) + '-' +
                     str(dateFrom.day) + 'T' +
                     str(timeFrom.hour) + '%3A' +
                     str(timeFrom.minute) + '%3A00.000Z' +
                     "&to=" + str(dateTo.year) + '-' +
                     str(dateTo.month) + '-' +
                     str(dateTo.day) + 'T' +
                     str(timeTo.hour) + '%3A' +
                     str(timeTo.minute) + '%3A00.000Z')
        return urlString
    
    def _makeDateFromStr(self, date_str):
        return datetime.strptime(date_str, "%d-%m-%Y")
    
    def _makeTimeFromStr(self, time_str):
        return datetime.strptime(time_str, "%H:%M:%S").time()
    
    def _makeDateTimeFromStr(self, date_str, time_str):
        return datetime.combine(self._makeDateFromStr(date_str), self._makeTimeFromStr(time_str))
    
    def getData(self, pv, dateFrom, timeFrom, dateTo, timeTo):
        archiver_link = self.archiver_root + self._formURLStr(pv, dateFrom, timeFrom, dateTo, timeTo)
        print("REQUESTING DATA FROM: ", archiver_link)
        req = urllib2.urlopen(archiver_link)
        data = json.load(req)
        return data