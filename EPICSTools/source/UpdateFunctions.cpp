#include <UpdateFunctions.h>
#include <Listener.h>


UpdateFunctionHolder::UpdateFunctionHolder()
{}

UpdateFunctionHolder::UpdateFunctionHolder(const UpdateFunctionHolder& copyUpdateFunctionHolder)
{}

UpdateFunctionPtr UpdateFunctionHolder::findUpdateFunction(pvStruct& pv)
{
	switch (ca_field_type(pv.CHID))
	{
	case(DBR_TIME_DOUBLE):
		if (pv.COUNT > 1)
		{
			return updateDoubleArray;
		}
		else
		{
			return updateDoubleValue;
		}
		break;
	case(DBR_DOUBLE):
		if (pv.COUNT > 1)
		{
			return updateDoubleArray;
		}
		else
		{
			return updateDoubleValue;
		}
		break;
	//case(DBR_TIME_INT):
	//	if (pv.COUNT > 1)
	//	{
	//		return updateIntegerArray;
	//	}
	//	else
	//	{
	//		return updateIntegerValue;
	//	}
	//	break;
	//case(DBR_INT):
	//	if (pv.COUNT > 1)
	//	{
	//		return updateIntegerArray;
	//	}
	//	else
	//	{
	//		return updateIntegerValue;
	//	}
	//	break;
	case(DBR_TIME_ENUM):
		if (pv.COUNT > 1)
		{
			return updateEnumArray;
		}
		else
		{
			return updateEnumValue;
		}
		break;
	case(DBR_ENUM):
		if (pv.COUNT > 1)
		{
			return updateEnumArray;
		}
		else
		{
			return updateEnumValue;
		}
		break;
	case(DBR_TIME_LONG):
		if (pv.COUNT > 1)
		{
			return updateLongArray;
		}
		else
		{
			return updateLongValue;
		}
		break;
	case(DBR_LONG):
		if (pv.COUNT > 1)
		{
			return updateLongArray;
		}
		else
		{
			return updateLongValue;
		}
		break;
	case(DBR_TIME_SHORT):
		if (pv.COUNT > 1)
		{
			return updateShortArray;
		}
		else
		{
			return updateShortValue;
		}
		break;
	case(DBR_SHORT):
		if (pv.COUNT > 1)
		{
			return updateShortArray;
		}
		else
		{
			return updateShortValue;
		}
		break;
	case(DBR_TIME_FLOAT):
		if (pv.COUNT > 1)
		{
			return updateFloatArray;
		}
		else
		{
			return updateFloatValue;
		}
		break;
	case(DBR_FLOAT):
		if (pv.COUNT > 1)
		{
			return updateFloatArray;
		}
		else
		{
			return updateFloatValue;
		}
		break;
	case(DBR_TIME_STRING):
		if (pv.COUNT > 1)
		{
			return updateStringArray;
		}
		else
		{
			return updateStringValue;
		}
		break;
	case(DBR_STRING):
		if (pv.COUNT > 1)
		{
			return updateStringArray;
		}
		else
		{
			return updateStringValue;
		}
		break;
	}
}

// COULD BE A PROBLEM THAT Listener cannot adapt its own value here..
// maybe moving these up
void UpdateFunctionHolder::updateDoubleValue(const struct event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, double> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampDoublePair(args, pairToUpdate);
	recastListener->setValue(pairToUpdate);
	recastListener->currentBuffer.push_back(pairToUpdate);
}

//void UpdateFunctionHolder::updateIntegerValue(const struct event_handler_args args)
//{
//	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
//	recastListener->callCount++;
//	std::pair<epicsTimeStamp, int> pairToUpdate = recastListener->epicsInterface->getTimeStampIntegerPair(args);
//	recastListener->setValue<int>(pairToUpdate.second);
//	recastListener->currentBuffer.push_back(pairToUpdate.second);
//	recastListener->messenger.printMessage("LISTENER VALUE: ", recastListener->getValue<int>());
//}

void UpdateFunctionHolder::updateShortValue(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, short> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampShortPair(args, pairToUpdate);
	recastListener->setValue(pairToUpdate);
	recastListener->currentBuffer.push_back(pairToUpdate);
}

void UpdateFunctionHolder::updateEnumValue(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, unsigned short> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampUShortPair(args, pairToUpdate);
	recastListener->setValue(pairToUpdate);
	recastListener->currentBuffer.push_back(pairToUpdate);
}

void UpdateFunctionHolder::updateFloatValue(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, float> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampFloatPair(args, pairToUpdate);
	recastListener->setValue(pairToUpdate);
	recastListener->currentBuffer.push_back(pairToUpdate);
}

void UpdateFunctionHolder::updateLongValue(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, long> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampLongPair(args, pairToUpdate);
	recastListener->setValue(pairToUpdate);
	recastListener->currentBuffer.push_back(pairToUpdate);
}

void UpdateFunctionHolder::updateStringValue(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, std::string> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampStringPair(args, pairToUpdate);
	recastListener->setValue(pairToUpdate);
	recastListener->currentBuffer.push_back(pairToUpdate);
}

void UpdateFunctionHolder::updateDoubleArray(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, std::vector<double>> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampDoubleVectorPair(args, pairToUpdate, recastListener->pv.COUNT);
	recastListener->setArray(pairToUpdate);
	timeStampVector bufferVec;
	for (auto& item : pairToUpdate.second)
	{
		bufferVec.second.push_back(item);
	}
	recastListener->currentArrayBuffer.push_back(bufferVec);
	recastListener->messenger.printMessage("LISTENER ARRAY UPDATED SIZE: ", recastListener->currentArray.second.size());
}

//void UpdateFunctionHolder::updateIntegerArray(const event_handler_args args)
//{
//	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
//	recastListener->callCount++;
//	std::pair<epicsTimeStamp, std::vector<int>> pairToUpdate;
//	recastListener->epicsInterface->updateTimeStampIntegerVectorPair(args, pairToUpdate, recastListener->pv.COUNT);
//	recastListener->setArray(pairToUpdate.second);
//	std::vector<boost::variant<double, float, long, int, unsigned short, std::string>> bufferVec;
//	for (auto& item : pairToUpdate.second)
//	{
//		bufferVec.push_back(item);
//	}
//	recastListener->currentArrayBuffer.push_back(bufferVec);
//	recastListener->messenger.printMessage("LISTENER ARRAY UPDATED SIZE: ", recastListener->currentArray.size());
//}

void UpdateFunctionHolder::updateShortArray(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, std::vector<short>> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampShortVectorPair(args, pairToUpdate, recastListener->pv.COUNT);
	recastListener->setArray(pairToUpdate);
	timeStampVector bufferVec;
	for (auto& item : pairToUpdate.second)
	{
		bufferVec.second.push_back(item);
	}
	recastListener->currentArrayBuffer.push_back(bufferVec);
	recastListener->messenger.printMessage("LISTENER ARRAY UPDATED SIZE: ", recastListener->currentArray.second.size());
}

void UpdateFunctionHolder::updateEnumArray(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, std::vector<unsigned short>> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampEnumVectorPair(args, pairToUpdate, recastListener->pv.COUNT);
	recastListener->setArray(pairToUpdate);
	timeStampVector bufferVec;
	for (auto& item : pairToUpdate.second)
	{
		bufferVec.second.push_back(item);
	}
	recastListener->currentArrayBuffer.push_back(bufferVec);
	recastListener->messenger.printMessage("LISTENER ARRAY UPDATED SIZE: ", recastListener->currentArray.second.size());

}

void UpdateFunctionHolder::updateFloatArray(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, std::vector<float>> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampFloatVectorPair(args, pairToUpdate, recastListener->pv.COUNT);
	recastListener->setArray(pairToUpdate);
	timeStampVector bufferVec;
	for (auto& item : pairToUpdate.second)
	{
		bufferVec.second.push_back(item);
	}
	recastListener->currentArrayBuffer.push_back(bufferVec);
	recastListener->messenger.printMessage("LISTENER ARRAY UPDATED SIZE: ", recastListener->currentArray.second.size());

}

void UpdateFunctionHolder::updateLongArray(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, std::vector<long>> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampLongVectorPair(args, pairToUpdate, recastListener->pv.COUNT);
	recastListener->setArray(pairToUpdate);
	timeStampVector bufferVec;
	for (auto& item : pairToUpdate.second)
	{
		bufferVec.second.push_back(item);
	}
	recastListener->currentArrayBuffer.push_back(bufferVec);
	recastListener->messenger.printMessage("LISTENER ARRAY UPDATED SIZE: ", recastListener->currentArray.second.size());
}

void UpdateFunctionHolder::updateStringArray(const event_handler_args args)
{
	Listener* recastListener = EPICSInterface::getHardwareFromArgs<Listener>(args);
	recastListener->callCount++;
	std::pair<epicsTimeStamp, std::vector<std::string>> pairToUpdate;
	recastListener->epicsInterface->updateTimeStampStringVectorPair(args, pairToUpdate, recastListener->pv.COUNT);
	recastListener->setArray(pairToUpdate);
	timeStampVector bufferVec;
	for (auto& item : pairToUpdate.second)
	{
		bufferVec.second.push_back(item);
	}
	recastListener->currentArrayBuffer.push_back(bufferVec);
	recastListener->messenger.printMessage("LISTENER ARRAY UPDATED SIZE: ", recastListener->currentArray.second.size());

}
