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
        
    def _getDateTimeFromTimestamp(self, timestamp):
        epochStamp = datetime.fromtimestamp(timestamp)
        return epochStamp.strftime('%d-%m-%Y %H:%M:%S')
    
    def getTimeFormattedData(self, pv, dateFrom, timeFrom, dateTo, timeTo):
        data = self.getData(pv, dateFrom, timeFrom, dateTo, timeTo)
        formattedData = {}
        for pv_key, archiver_data in data.items():
            ## item contains 'meta' and 'data' keys
            formattedData[pv_key] = {}
            for values in archiver_data:
                for raw_data in values["data"]:
                    timeStr = self._getDateTimeFromTimestamp(raw_data["secs"])
                    formattedData[pv_key][timeStr] = raw_data["val"]
        return formattedData
    
    def getEpochFormattedData(self, pv, dateFrom, timeFrom, dateTo, timeTo):
        data = self.getData(pv, dateFrom, timeFrom, dateTo, timeTo)
        formattedData = {}
        for pv_key, archiver_data in data.items():
            ## item contains 'meta' and 'data' keys
            formattedData[pv_key] = {}
            for values in archiver_data:
                for raw_data in values["data"]:
                    timeStr = raw_data["secs"]
                    formattedData[pv_key][timeStr] = raw_data["val"]
        return formattedData
    
    def getData(self, pv, dateFrom, timeFrom, dateTo, timeTo):
        dataDict = {}
        if type(pv) is list:
            for item in pv:
                dataDict[item] = {}
                archiver_link = self.archiver_root + self._formURLStr(item, dateFrom, timeFrom, dateTo, timeTo)
                print("REQUESTING DATA FROM: ", archiver_link)
                req = urllib2.urlopen(archiver_link)
                dataDict[item] = json.load(req)
            return dataDict
        elif type(pv) is str:
            archiver_link = self.archiver_root + self._formURLStr(pv, dateFrom, timeFrom, dateTo, timeTo)
            print("REQUESTING DATA FROM: ", archiver_link)
            req = urllib2.urlopen(archiver_link)
            dataDict[pv] = json.load(req)
            return dataDict
        else:
            print("Argument \"PV\" is " + str(type(pv)) + " please try list or str type")
            return {}