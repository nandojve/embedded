/* Prevent double inclusion */
#ifndef ARM_DRIVERS_H
#define ARM_DRIVERS_H

#define		TRUE	1
#define		FALSE	0

//_____  I N C L U D E S ___________________________________________________

//_____ M A C R O S ________________________________________________________


//_____ P R I V A T E    D E C L A R A T I O N _____________________________


//_____ D E F I N I T I O N ________________________________________________


//_____ D E C L A R A T I O N ______________________________________________
#ifdef __cplusplus
extern "C" {
#endif

void driver_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif  /* ARM_DRIVERS_H */
/* EOF */