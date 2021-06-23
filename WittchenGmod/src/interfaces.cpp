#include "interfaces.h"

#include "utils/memoryutils.h"

void interfaces::InitInterfaces() {
	Engine = MemoryUtils::CaptureInterface<CEngineClient>("engine.dll", "VEngineClient015");
	Client = MemoryUtils::CaptureInterface<CHLClient>("client.dll", "VClient017");
	EntityList = MemoryUtils::CaptureInterface<CClientEntityList>("client.dll", "VClientEntityList003");

	
}
