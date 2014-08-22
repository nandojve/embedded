/*
 * LwMesh_SimpleRemote.cpp
 *
 * Created: 28/05/2014 10:29:58
 *  Author: nando
 */ 

// include the library code:
#include <Arduino.h>
#include <LwMesh.h>

LwMesh lwMesh;

#ifdef __cplusplus
extern "C" {
#endif

	bool rx_frame(NWK_DataInd_t *ind);
	void app_task(void);

#ifdef __cplusplus
}
#endif

void setup()
{
	lwMesh.begin(rx_frame, 0);
}

void loop()
{
	lwMesh.task();
	app_task();
}