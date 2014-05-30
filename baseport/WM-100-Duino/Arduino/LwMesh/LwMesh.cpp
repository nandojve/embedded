#include "LwMesh.h"
#include "sys.h"
#include "nwk.h"
#include "phy.h"

void LwMesh::begin(bool (*handler)(NWK_DataInd_t *ind), uint16_t netaddr, uint16_t panid, uint8_t channel)
{
	SYS_Init();

	NWK_SetAddr(netaddr);
	NWK_SetPanId(panid);
	PHY_SetChannel(channel);
	PHY_SetTxPower(0x00);		// Max Power
	PHY_SetRxState(true);

	NWK_OpenEndpoint(LwMeshDefaultEndPoint, handler);
}
void LwMesh::task(void)
{
	SYS_TaskHandler();
}