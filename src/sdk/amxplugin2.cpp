//----------------------------------------------------------
//
//   SA-MP Multiplayer Modification For GTA:SA
//   Copyright 2013 SA-MP Team, Dan
//
//----------------------------------------------------------

#include <cstring>
#include <cstdlib>

//----------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------

#define HAVE_STDINT_H
#include "amx/amx2.h"

//----------------------------------------------------------

int AMXAPI amx_PushAddress(AMX *amx, cell *address) {
	AMX_HEADER *hdr;
	unsigned char *data;
	cell xaddr;
	/* reverse relocate the address */
	assert(amx != NULL);
	hdr = (AMX_HEADER *) amx->base;
	assert(hdr != NULL);
	assert(hdr->magic == AMX_MAGIC);
	data = (amx->data != NULL) ? amx->data : amx->base + (int) hdr->dat;
	xaddr = (cell) ((unsigned char*) address-data);
	if ((ucell) xaddr >= (ucell) amx->stp) {
		return AMX_ERR_MEMACCESS;
	}
	return amx_Push(amx,xaddr);
}

void AMXAPI amx_Redirect(AMX *amx, const char *from, ucell to, AMX_NATIVE *store) {
	AMX_HEADER *hdr = (AMX_HEADER*) amx->base;
	AMX_FUNCSTUB *func;
	for (int idx = 0, num = NUMENTRIES(hdr, natives, libraries); idx != num; ++idx) {
		func = GETENTRY(hdr, natives, idx);
		if (!strcmp(from, GETENTRYNAME(hdr, func))) {
			if (store) {
				*store = (AMX_NATIVE) func->address;
			}
			func->address = to;
			return;
		}
	}
}

int AMXAPI amx_GetCString(AMX *amx, cell param, char *&dest) {
	cell *ptr;
	amx_GetAddr(amx, param, &ptr);
	int len;
	amx_StrLen(ptr, &len);
	dest = (char*) malloc((len + 1) * sizeof(char));
	if (dest != NULL) {
		amx_GetString(dest, ptr, 0, UNLIMITED);
		dest[len] = 0;
		return len;
	}
	return 0;
}

void AMXAPI amx_SetCString(AMX *amx, cell param, const char *str, int len) {
	cell *dest;
	amx_GetAddr(amx, param, &dest);
	amx_SetString(dest, str, 0, 0, len);
}

std::string AMXAPI amx_GetCppString(AMX *amx, cell param) {
	char *tmp;
	amx_StrParam(amx, param, tmp);
	if (tmp != NULL) {
		return tmp;
	}
	return "";
}

void AMXAPI amx_SetCppString(AMX *amx, cell param, std::string str, int len) {
	cell *dest;
	amx_GetAddr(amx, param, &dest);
	amx_SetString(dest, str.c_str(), 0, 0, len);
}

//----------------------------------------------------------
// EOF
