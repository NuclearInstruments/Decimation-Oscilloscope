// Il seguente blocco ifdef rappresenta il modo standard di creare macro che semplificano 
// l'esportazione da una DLL. Tutti i file all'interno della DLL sono compilati con il simbolo R5560_SDKLIB_EXPORTS
// definito nella riga di comando. Questo simbolo non deve essere definito in alcun progetto
// che utilizza questa DLL. In questo modo qualsiasi altro progetto i cui file di origine includono questo file vedranno le funzioni 
// R5560_SDKLIB_API come importate da una DLL, mentre la DLL vedrà i simboli
// definiti con questa macro come esportati.


#ifdef __cplusplus
#ifdef NIUSB3_CORE_EXPORTS
#define R5560_SDKLIB_API extern "C" __declspec(dllexport)
#else
#define R5560_SDKLIB_API extern "C" __declspec(dllimport)
#endif
#else
#define R5560_SDKLIB_API
#endif

#define NO_ERROR 0
#define ERR_NO_CONNECTION -1
#define ERR_SOCKET -2
#define ERR_CONNECTION_DROP -3
#define ERR_ALLOC -4
#define ERR_OUT_OF_LIMIT -5
#define ERR_TIMEOUT -6

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "Def.h"


R5560_SDKLIB_API int R5560_ConnectTCP(char *ipaddress, uint32_t port, tR5560_Handle *handle);
R5560_SDKLIB_API int NI_CloseConnection(tR5560_Handle *handle);
R5560_SDKLIB_API int NI_WriteData(uint32_t *data, uint32_t count, 
										uint32_t address, 
										tR5560_Handle *handle,
										uint32_t *written_data);
R5560_SDKLIB_API int NI_ReadData(uint32_t *data, uint32_t count, 
										uint32_t address, tR5560_Handle *handle, 
										uint32_t *read_data);
R5560_SDKLIB_API int NI_ReadFifo(uint32_t *data, uint32_t count, 
										uint32_t address, uint32_t fifo_status_address, BUS_MODE bus_mode, 
										uint32_t timeout_ms, tR5560_Handle *handle, 
										uint32_t *read_data);
R5560_SDKLIB_API int NI_WriteReg(uint32_t data, uint32_t address, tR5560_Handle *handle);
R5560_SDKLIB_API int NI_ReadReg(uint32_t *data, uint32_t address, tR5560_Handle *handle);

R5560_SDKLIB_API int NI_InternalWriteReg(uint32_t data, uint32_t address, tR5560_Handle *handle);
R5560_SDKLIB_API int NI_InternalReadReg(uint32_t *data, uint32_t address, tR5560_Handle *handle);