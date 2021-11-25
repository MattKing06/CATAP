import urllib.request as urllib2
import json
from datetime import datetime, date, time, timedelta


"""
EPICS Archiver appliance default binning size is 900 second (15 minutes)
This is now a configurable parameter, the small the value, the more data,
the more strain on the archiver. Use this wisely.
 """
DEFAULT_CHUNK_SIZE = 900

class EPICSArchiver(object):

    def __init__(self):
        """
        EPICSArchiver is getting data in JSON format from the following location:
        "http://claraserv2.dl.ac.uk:17668/retrieval/data/"
        """
        self.archiver_root = "http://claraserv2.dl.ac.uk:17668/retrieval/data/"


    def _printMessage(self, message):
        print(message)
        
    ### TIME IS FORMAT T08%3A00%3A00.000Z : T<HOURS>%3A<MINS>%3A<SECONDS>.000<ZONE>

    def _formURLStr(self, pv, dateFrom, timeFrom, dateTo, timeTo, chunking=DEFAULT_CHUNK_SIZE):
        if chunking >= 1:
            pv = pv.replace(':', '%3A')
            urlString = ("getData.json?pv="+ "mean_"+str(chunking) + "(" + str(pv) +")" +
                         "&from=" + str(dateFrom.year) +
                         "-" + str(dateFrom.month) + '-' +
                         str(dateFrom.day) + 'T' +
                         str(timeFrom.hour) + '%3A' +
                         str(timeFrom.minute) + '%3A'+
                         str(timeFrom.second) + '.000Z' +
                         "&to=" + str(dateTo.year) + '-' +
                         str(dateTo.month) + '-' +
                         str(dateTo.day) + 'T' +
                         str(timeTo.hour) + '%3A' +
                         str(timeTo.minute) + '%3A' +
                         str(timeTo.second) + '.000Z')
        else:
            pv = pv.replace(':', '%3A')
            fromMillis = round(timeFrom.microsecond/1000)
            toMillis = round(timeTo.microsecond/1000)
            urlString = ("getData.json?pv="+ str(pv) +
                         "&from=" + str(dateFrom.year) +
                         "-" + str(dateFrom.month) + '-' +
                         str(dateFrom.day) + 'T' +
                         str(timeFrom.hour) + '%3A' +
                         str(timeFrom.minute) + '%3A'+
                         str(timeFrom.second) +'.'+str(fromMillis) + 'Z' +
                         "&to=" + str(dateTo.year) + '-' +
                         str(dateTo.month) + '-' +
                         str(dateTo.day) + 'T' +
                         str(timeTo.hour) + '%3A' +
                         str(timeTo.minute) + '%3A' +
                         str(timeTo.second) +'.' + str(toMillis) + 'Z')
        return urlString

    def _makeDateFromStr(self, date_str):
        return datetime.strptime(date_str, "%d-%m-%Y")

    def _makeTimeFromStr(self, time_str):
        return datetime.strptime(time_str, "%H:%M:%S.%f").time()

    def _makeDateTimeFromStr(self, date_str, time_str):
        return datetime.combine(self._makeDateFromStr(date_str), self._makeTimeFromStr(time_str))

    def _getDateTimeFromTimestamp(self, timestamp):
        epochStamp = timestamp #datetime.fromtimestamp(timestamp)
        return epochStamp.strftime('%d-%m-%Y %H:%M:%S.%f')

    def dumpDataToFile(self, pv, dateFrom, timeFrom, dateTo, timeTo, chunking=DEFAULT_CHUNK_SIZE, filename='archiver_data.json'):
        """
        Puts the data from archiver request (pv, dates and times) into a .json file
        
        Dates and times: dateFrom (datetime.date), timeFrom (datetime.time), dateTo (datetime.date), timeTo (datetime.time)

        Default Chunking size (int) is 900secs (15 minutes) and the mean of the value in 15 mins is retrieved.

        Default File name is archiver_data.json (not unique)
        """
        data = self.getData(pv, dateFrom, timeFrom, dateTo, timeTo)
        with open(filename, 'w') as file:
            json.dump(data, file)

    def getTimeFormattedData(self, pv, dateFrom, timeFrom, dateTo, timeTo, chunking):
        """
        Returns the data from archiver request (pv, dates and times) as a dict with entries {<time-stamp> : <value>}
        
        Dates and times: dateFrom (datetime.date), timeFrom (datetime.time), dateTo (datetime.date), timeTo (datetime.time)
        
        Default Chunking size (int) is 900secs (15 minutes) and the mean of the value in 15 mins is retrieved.

        Time format is: %d-%m-%Y %H:%M:%S.%f (DD-MM-YYYY HH:MM:SS.ffff)
        """
        data = self.getData(pv, dateFrom, timeFrom, dateTo, timeTo, chunking)
        formattedData = {}
        for pv_key, archiver_data in data.items():
            ## item contains 'meta' and 'data' keys
            formattedData[pv_key] = {}
            for values in archiver_data:
                for raw_data in values["data"]:
                    # ADD THE NANOS (as microseconds) TO RAW_DATA[SECS] TO GET THE TIME BETWEEN SECONDS.
                    offset_time = datetime.fromtimestamp(raw_data["secs"]) + timedelta(microseconds=(raw_data["nanos"]/1000))
                    timeStr = self._getDateTimeFromTimestamp(offset_time)
                    formattedData[pv_key][timeStr] = raw_data["val"]
        return formattedData

    def getEpochSecsFormattedData(self, pv, dateFrom, timeFrom, dateTo, timeTo, chunking=DEFAULT_CHUNK_SIZE):
        """
        Returns the data from archiver request (pv, dates and times) as a dict with entries {<epoch-secs> : <value>}
        
        Dates and times: dateFrom (datetime.date), timeFrom (datetime.time), dateTo (datetime.date), timeTo (datetime.time)
        
        Default Chunking size (int) is 900secs (15 minutes) and the mean of the value in 15 mins is retrieved.

        Epoch in seconds
        """
        data = self.getData(pv, dateFrom, timeFrom, dateTo, timeTo, chunking)
        formattedData = {}
        for pv_key, archiver_data in data.items():
            ## item contains 'meta' and 'data' keys
            formattedData[pv_key] = {}
            for values in archiver_data:
                for raw_data in values["data"]:
                    timeStr = raw_data["secs"]
                    formattedData[pv_key][timeStr] = raw_data["val"]
        return formattedData

    # def getEpochNanosFormattedData(self, pv, dateFrom, timeFrom, dateTo, timeTo, chunking=DEFAULT_CHUNK_SIZE):
        # """
        # Returns the data from archiver request (pv, dates and times) as a dict with entries {<epoch-nanos> : <value>}
        
        # Dates and times: dateFrom (datetime.date), timeFrom (datetime.time), dateTo (datetime.date), timeTo (datetime.time)
        
        # Default Chunking size (int) is 900 secs (15 minutes) and the mean of the value in 15 mins is retrieved.

        # Epoch in nanos
        # """
        # data = self.getData(pv, dateFrom, timeFrom, dateTo, timeTo, chunking)
        # formattedData = {}
        # for pv_key, archiver_data in data.items():
            # ## item contains 'meta' and 'data' keys
            # formattedData[pv_key] = {}
            # for values in archiver_data:
                # for raw_data in values["data"]:
                    # timeStr = raw_data["nanos"]
                    # formattedData[pv_key][timeStr] = raw_data["val"]
        # return formattedData

    def getData(self, pv, dateFrom, timeFrom, dateTo, timeTo, chunking=DEFAULT_CHUNK_SIZE):
        """
        Returns the data from archiver request (pv, dates and times) with all metadata and data.
        
        Dates and times: dateFrom (datetime.date), timeFrom (datetime.time), dateTo (datetime.date), timeTo (datetime.time)
        
        Default Chunking size (int) is 900 secs (15 minutes) and the mean of the value in 15 mins is retrieved.

        return dict: first entry is 'meta' and second entry is 'data'
        """
        dataDict = {}
        if type(pv) is list:
            for item in pv:
                dataDict[item] = {}
                archiver_link = self.archiver_root + self._formURLStr(item, dateFrom, timeFrom, dateTo, timeTo, chunking)
                print("REQUESTING DATA FROM: ", archiver_link)
                req = urllib2.urlopen(archiver_link)
                dataDict[item] = json.load(req)
            return dataDict
        elif type(pv) is str:
            archiver_link = self.archiver_root + self._formURLStr(pv, dateFrom, timeFrom, dateTo, timeTo, chunking)
            print("REQUESTING DATA FROM: ", archiver_link)
            req = urllib2.urlopen(archiver_link)
            dataDict[pv] = json.load(req)
            return dataDict
        else:
            print("Argument \"PV\" is " + str(type(pv)) + " please try list or str type")
            return {}