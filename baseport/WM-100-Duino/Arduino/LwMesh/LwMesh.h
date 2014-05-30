#include <stdint.h>
#include <nwkRx.h>

#define LwMeshDefaultEndPoint 1

class LwMesh
{
	public:
		void begin(bool (*handler)(NWK_DataInd_t *ind), uint16_t netaddr, uint16_t panid = 0xCAFE, uint8_t channel = 0x0F);
		void task(void);

	public:

};